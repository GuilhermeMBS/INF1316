/**
 * 7) Elabore três programas I/O bound que não terminem (loop de msgs no vídeo).
 * Elabore um programa que seja capaz de executar os 3 programas indicados anteriormente
 * e que simule o compartilhamento da CPU entre os 3 processos com escalonamento Round-Robin
 * com uma fatia de tempo de 1 segundo para o primeiro processo e de 2 segundos para os demais
 * processos. Execute os programas e relate o que aconteceu.
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


void make_call(ull time) {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = (ull)time * 1e6;
    pid_t ppid = getppid();

    puts("[Call started]");
    kill(ppid, SIGUSR1);
    nanosleep(&ts, NULL);
    puts("[Call ended]");
    kill(ppid, SIGUSR2);
}


void handle_sig(int signal) {
    if (signal == SIGUSR1) {
        timespec_get(&start, TIME_UTC);
    }

    if (signal == SIGUSR2) {
        timespec_get(&end, TIME_UTC);
        double elapsed = (end.tv_sec - start.tv_sec) * 1000.0 +
                         (end.tv_nsec - start.tv_nsec) / 1e6;
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

    pid_t pid = fork();

    if (pid == 0) {
        make_call(90); // One and a half minute
        exit(1);
    }
    else if (pid < 0) perror("Error while forking the process");

    waitpid(pid, NULL, 0);

    return 0;
}
