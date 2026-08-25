/*
    1) Faça um programa para criar dois processos, o pai escreve seu pid e espera o
filho terminar e o filho escreve o seu pid e termina.
*/
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int mypid, pid, status;
    pid = fork();
    if (pid != 0) {
        // Pai
        mypid = getpid();
        printf("PID pai: %d\n", mypid);
        waitpid(-1, &status, 0);
    }
    else {
        // Filho
        mypid = getpid();
        printf("PID filho: %d\n", mypid);
        exit(3);
    }
    return 0;
}