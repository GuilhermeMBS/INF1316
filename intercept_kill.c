/*
* 2) Tente fazer um programa para interceptar o
* sinal SIGKILL. Você conseguiu? Explique.
*/

// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <sys/stat.h>

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define EVER ;;


void funcaoTratadora (int sinal) {
    printf("Sinal: %d\n", sinal);

    switch (sinal) {
        case SIGKILL:
            puts("SIGKILL Was Triggered!\n");
            break;
            
        case SIGILL:
            puts("Illegal Instruction!\n");
            break;
            
        default:
            puts("Signal Triggered\n");
            exit(0);
    }
}

int main (void) {
    if (signal(SIGKILL, funcaoTratadora) == SIG_ERR) {
        puts("Handler Allocation Error\n");
        exit(1);
    }
    
    raise(SIGKILL);
    raise(SIGILL);

    return 0;
}
