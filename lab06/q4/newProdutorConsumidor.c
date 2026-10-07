#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAXFILA 8
#define TOTAL_ELEMENTOS 64
#define NUM_PRODUTORES 2
#define NUM_CONSUMIDORES 2


int fila[MAXFILA];
int in = 0, out = 0, count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_produtor = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumidor = PTHREAD_COND_INITIALIZER;

void* produtor(void* arg) {
    long id = (long)arg;
    int qtd_por_produtor = TOTAL_ELEMENTOS / NUM_PRODUTORES;

    for (int i = 0; i < qtd_por_produtor; i++) {
        int item = rand() % 100; 

        pthread_mutex_lock(&mutex);

        while (count == MAXFILA) {
            pthread_cond_wait(&cond_produtor, &mutex);
        }

        fila[in] = item;
        printf("[PRODUTOR %ld] Inseriu: %d \t(Pos: %d | Total: %d)\n", id, item, in, count + 1);
        in = (in + 1) % MAXFILA;
        count++;

        pthread_cond_broadcast(&cond_consumidor);
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
    return NULL;
}

void* consumidor(void* arg) {
    long id = (long)arg;
    int qtd_por_consumidor = TOTAL_ELEMENTOS / NUM_CONSUMIDORES;

    for (int i = 0; i < qtd_por_consumidor; i++) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            pthread_cond_wait(&cond_consumidor, &mutex);
        }

        int item = fila[out];
        printf("\t[CONSUMIDOR %ld] Retirou: %d \t(Pos: %d | Total: %d)\n", id, item, out, count - 1);
        out = (out + 1) % MAXFILA;
        count--;

        pthread_cond_broadcast(&cond_produtor);
        pthread_mutex_unlock(&mutex);

        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t produtores[NUM_PRODUTORES];
    pthread_t consumidores[NUM_CONSUMIDORES];
    srand(time(NULL));

    for (long i = 0; i < NUM_PRODUTORES; i++) {
        pthread_create(&produtores[i], NULL, produtor, (void*)(i + 1));
    }
    for (long i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_create(&consumidores[i], NULL, consumidor, (void*)(i + 1));
    }

    for (int i = 0; i < NUM_PRODUTORES; i++) {
        pthread_join(produtores[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMIDORES; i++) {
        pthread_join(consumidores[i], NULL);
    }

    return 0;
}
