/**
 * Our program uses a time abstraction so that 1 ms represents 1 second 
 * of simulated phone call.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>

// Ensures atomic access and prevents compiler register caching
typedef volatile sig_atomic_t Hflag;

static Hflag call_started = 0;
static Hflag call_ended = 0;

static struct timespec start, end;


void estimate_price(int call_id, double duration) {
    double price = duration * 2.0;
    if (duration > 60.0) price -= (duration - 60.0);
    printf("=> Price: R$ %.2f\n\n", price / 100.0);
}


void handle_child(int sig) {
    if (sig == SIGUSR1) {
        timespec_get(&start, TIME_UTC);
        call_started = 1;
    }

    else if (sig == SIGUSR2) {
        timespec_get(&end, TIME_UTC);
        call_ended = 1;
    }
}


void child_routine(int call_id) {
    signal(SIGUSR1, handle_child);
    signal(SIGUSR2, handle_child);

    while (!call_started) pause();
    printf("[Call %d] Started\n", call_id);

    while (!call_ended) pause();
    double elapsed = (end.tv_sec - start.tv_sec) * 1000.0 +
                     (end.tv_nsec - start.tv_nsec) / 1e6;
    printf("[Call %d] Ended With %.2f\n", call_id, elapsed);
    estimate_price(call_id, elapsed);

    exit(0);
}


int main(void) {
    pid_t pids[3];

    for (int i = 0; i < 3; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("Fork failed");
            exit(1);
        }

        else if (pids[i] == 0) child_routine(i + 1);
    }

    usleep(20000); // Ensure all children have registered their signal handlers
    for (int i = 0; i < 3; i++) kill(pids[i], SIGUSR1);

    // 55 seconds call (should cost around R$1,10)
    usleep(55 * 1000);
    kill(pids[1], SIGUSR2);

    // One minute and a half long call (should cost around R$1,50)
    usleep(35 * 1000); // (90 - 55) seconds
    kill(pids[0], SIGUSR2);

    // 5 minutes call (should cost around R$3,60)
    usleep(210 * 1000); // (300 - 55 - 35) seconds
    kill(pids[2], SIGUSR2);

    for (int i = 0; i < 3; i++) waitpid(pids[i], NULL, 0);

    return 0;
}
