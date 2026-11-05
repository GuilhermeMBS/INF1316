#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/time.h>


#define WRITE_THROUGH 1 // Change to 1 for Write-Through policy
#define CACHE_SIZE 100
#define DB_SIZE 1000
#define SIM_DELAY_US 100000 // 100ms delay simulating slow DB


typedef enum
{
    CMD_READ,
    CMD_WRITE,
    CMD_INVALIDATE
} cmd_type_t;

typedef struct {
    int key;
    int value;
    unsigned long long timestamp;
    int is_valid;
} cache_entry_t;

static cache_entry_t g_cache[CACHE_SIZE];
static int g_db[DB_SIZE];
static unsigned long long g_lru_clock = 0;
static volatile sig_atomic_t g_clear_cache = 0;

// Inter-process Communication for Database
static pthread_mutex_t db_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t db_cv_req = PTHREAD_COND_INITIALIZER;
static pthread_cond_t db_cv_res = PTHREAD_COND_INITIALIZER;
static cmd_type_t db_req_cmd;
static int db_req_key, db_req_val, db_res_val;
static bool db_req_pending = false;

// Inter-process Communication for Cache
static pthread_mutex_t cache_mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cache_cv_req = PTHREAD_COND_INITIALIZER;
static pthread_cond_t cache_cv_res = PTHREAD_COND_INITIALIZER;
static cmd_type_t cache_req_cmd;
static int cache_req_key, cache_req_val, cache_res_val;
static bool cache_req_pending = false;


void
sigtstp_handler(int signo)
{
    if (signo == SIGTSTP) {
        g_clear_cache = 1;
        // Wakes up the cache thread immediately to flush
        pthread_cond_signal(&cache_cv_req); 
    }
}


int
db_request(cmd_type_t cmd, int key, int val)
{
    pthread_mutex_lock(&db_mtx);
    db_req_cmd = cmd;
    db_req_key = key;
    db_req_val = val;
    db_req_pending = true;
    
    pthread_cond_signal(&db_cv_req);
    while (db_req_pending) {
        pthread_cond_wait(&db_cv_res, &db_mtx);
    }
    
    int res = db_res_val;
    pthread_mutex_unlock(&db_mtx);
    return res;
}


int
cache_request(cmd_type_t cmd, int key, int val)
{
    pthread_mutex_lock(&cache_mtx);
    cache_req_cmd = cmd;
    cache_req_key = key;
    cache_req_val = val;
    cache_req_pending = true;
    
    pthread_cond_signal(&cache_cv_req);
    while (cache_req_pending) {
        pthread_cond_wait(&cache_cv_res, &cache_mtx);
    }
    
    int res = cache_res_val;
    pthread_mutex_unlock(&cache_mtx);
    return res;
}


void *
db_thread(void *arg)
{
    (void)arg;
    for (int i = 0; i < DB_SIZE; i++) {
        g_db[i] = i + 1;
    }

    while (1) {
        pthread_mutex_lock(&db_mtx);
        while (!db_req_pending) {
            pthread_cond_wait(&db_cv_req, &db_mtx);
        }
        
        // Simulates the slow disk I/O bottleneck
        usleep(SIM_DELAY_US); 
        
        if (db_req_cmd == CMD_READ) {
            db_res_val = g_db[db_req_key];
            printf("[DB] Read key %d = %d\n", db_req_key, db_res_val);
        } else if (db_req_cmd == CMD_WRITE) {
            g_db[db_req_key] = db_req_val;
            db_res_val = 1;
            printf("[DB] Wrote key %d = %d\n", db_req_key, db_req_val);
        }
        
        db_req_pending = false;
        pthread_cond_signal(&db_cv_res);
        pthread_mutex_unlock(&db_mtx);
    }
    return NULL;
}


void *
cache_thread(void *arg)
{
    (void)arg;
    for (int i = 0; i < CACHE_SIZE; i++) {
        g_cache[i].is_valid = 0;
    }

    while (1) {
        pthread_mutex_lock(&cache_mtx);
        
        // Block until there's a request OR a flush signal from Ctrl-Z
        while (!cache_req_pending && !g_clear_cache) {
            pthread_cond_wait(&cache_cv_req, &cache_mtx);
        }
        
        if (g_clear_cache) {
            for (int i = 0; i < CACHE_SIZE; i++) {
                g_cache[i].is_valid = 0;
            }
            g_clear_cache = 0;
            printf("\n=============================\n");
            printf("[CACHE] FLUSHED BY CTRL-Z\n");
            printf("=============================\n\n");
            
            // If woken up purely by Ctrl-Z, go back to sleep
            if (!cache_req_pending) {
                pthread_mutex_unlock(&cache_mtx);
                continue;
            }
        }

        cache_res_val = -1;
        
        if (cache_req_cmd == CMD_READ) {
            for (int i = 0; i < CACHE_SIZE; i++) {
                if (g_cache[i].is_valid && g_cache[i].key == cache_req_key) {
                    g_cache[i].timestamp = ++g_lru_clock;
                    cache_res_val = g_cache[i].value;
                    printf("[CACHE] Hit key %d\n", cache_req_key);
                    break;
                }
            }
            if (cache_res_val == -1) {
                printf("[CACHE] Miss key %d\n", cache_req_key);
            }
        } 
        else if (cache_req_cmd == CMD_WRITE) {
            int target = -1;
            unsigned long long oldest = (unsigned long long)-1;
            for (int i = 0; i < CACHE_SIZE; i++) {
                if (g_cache[i].is_valid && g_cache[i].key == cache_req_key) { 
                    target = i; 
                    break; 
                }
                if (!g_cache[i].is_valid) { 
                    target = i; 
                    break; 
                }
                if (g_cache[i].timestamp < oldest) { 
                    oldest = g_cache[i].timestamp; 
                    target = i; 
                }
            }
            g_cache[target].key = cache_req_key;
            g_cache[target].value = cache_req_val;
            g_cache[target].timestamp = ++g_lru_clock;
            g_cache[target].is_valid = 1;
            cache_res_val = 1;
        }
        else if (cache_req_cmd == CMD_INVALIDATE) {
            for (int i = 0; i < CACHE_SIZE; i++) {
                if (g_cache[i].is_valid && g_cache[i].key == cache_req_key) {
                    g_cache[i].is_valid = 0;
                    printf("[CACHE] Invalidated key %d\n", cache_req_key);
                    break;
                }
            }
            cache_res_val = 1;
        }
        
        cache_req_pending = false;
        pthread_cond_signal(&cache_cv_res);
        pthread_mutex_unlock(&cache_mtx);
    }
    return NULL;
}


void *
client_thread(void *arg)
{
    (void)arg;
    FILE *f = fopen("log.txt", "r");
    if (!f) {
        perror("Error opening log.txt");
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;
    int ops_processed = 0;
    bool benchmark_done = false;
    
    gettimeofday(&start, NULL);

    while (1) {
        char op;
        int k, v, res;
        
        if (fscanf(f, " %c", &op) != 1) {
            rewind(f);
            continue;
        }

        if (op == 'R') {
            if (fscanf(f, "%d", &k) == 1) {
                if (cache_request(CMD_READ, k, 0) == -1) {
                    res = db_request(CMD_READ, k, 0); 
                    cache_request(CMD_WRITE, k, res); 
                }
            }
        } else if (op == 'W') {
            if (fscanf(f, "%d %d", &v, &k) == 2) {
#if WRITE_THROUGH == 1
                cache_request(CMD_WRITE, k, v);
                db_request(CMD_WRITE, k, v);
#else
                db_request(CMD_WRITE, k, v);
                cache_request(CMD_INVALIDATE, k, 0);
#endif
            }
        }
        
        ops_processed++;
        if (ops_processed == 500 && !benchmark_done) {
            gettimeofday(&end, NULL);
            double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
            printf("\n======================================================\n");
            printf("[BENCHMARK] 500 ops processed in %.4f seconds.\n", elapsed);
            printf("======================================================\n\n");
            benchmark_done = true;
        }
        
        // Small sleep just so the terminal doesn't crash from pure IO spam,
        // but small enough that DB latency dominates the benchmark.
        usleep(500); 
    }
    return NULL;
}


int
main(void)
{
#if WRITE_THROUGH == 1
    printf("--- Starting WRITE-THROUGH Simulation ---\n");
#else
    printf("--- Starting CACHE-ASIDE Simulation ---\n");
#endif

    struct sigaction sa;
    sa.sa_handler = sigtstp_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa, NULL);

    pthread_t t_cache, t_db, t_client;
    pthread_create(&t_cache, NULL, cache_thread, NULL);
    pthread_create(&t_db, NULL, db_thread, NULL);
    pthread_create(&t_client, NULL, client_thread, NULL);
    
    pthread_join(t_client, NULL);
    return 0;
}
