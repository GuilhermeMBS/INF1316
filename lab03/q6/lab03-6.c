/**
 * This program uses milliseconds as an abstraction for the seconds.
 * So a call with 30 seconds long will be represented as a
 * 30 milliseconds long call in here.
 */
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


struct timespec start, end;
typedef unsigned long long ull;


void estimate_price(double duration) {
    double price = 0.0;
    price = (duration * 2.0);
    if (duration > 60) (price -= (duration - 60.0));
    printf("=> Estimated Price: R$%.2lf\n", price / 100.0);
}


void make_call(ull time, int call) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = time * 1e6;
    pid_t ppid = getppid();

    printf("[Call %d started]\n", call);
    kill(ppid, SIGUSR1);
    nanosleep(&ts, NULL);
    printf("[Call %d ended]\n", call);
    kill(ppid, SIGUSR2);
}


void handle_sig(int signal) {
    if (signal == SIGUSR1) {
        timespec_get(&start, TIME_UTC);
    }

    if (signal == SIGUSR2) {
        timespec_get(&end, TIME_UTC);
        double elapsed = (end.tv_nsec - start.tv_nsec) / 1e6;
        printf("The call had %.2f seconds. Estimating it's price...\n", elapsed);
        estimate_price(elapsed);
    }
}


int main(void) {
    if (signal(SIGUSR1, handle_sig) == SIG_ERR) {
        perror("Error while starting a call.");
        exit(1);
    }

    if (signal(SIGUSR2, handle_sig) == SIG_ERR) {
        perror("Error while ending the call");
        exit(1);
    }

    pid_t pids[3];
    ull times[3] = {
        1*60 + 30,  // One minute and a half long call (should cost around R$1,50)
        0*60 + 55,  // 55 seconds call (should cost around R$1,10)
        5*60 + 00   // 5 minutes call (should cost around R$3,60)
    };

    for (int i = 0; i < 3; i++) {
        pids[i] = fork();
        
        if (pids[i] == 0) {
            make_call(times[i], i + 1);
            exit(1);
        }
        else if (pids[i] < 0) perror("Error while forking the process");
    }

    for (int i = 0; i < 3; i++) waitpid(pids[i], NULL, 0);

    return 0;
}
