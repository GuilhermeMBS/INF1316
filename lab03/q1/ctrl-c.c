/*
* 1) Execute o programa “ctrl-c.c”.
* Digite Ctrl-C e Ctrl-\. Analise o resultado.
*/

// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <sys/stat.h>
// #include <unistd.h>
// #include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


void funcaoTratadora (int sinal) {
    // tratamento do(s) sinal(is)
}

int main (void) {
    // captura Ctrl-C (SIGINT)
    if (signal(SIGINT, funcaoTratadora) == SIG_ERR) {
        // erro ao instalar a rotina de atendimento do sinal SIGINT
    }
    // captura Ctrl-\ (SIGINT)
    else if (signal(SIGABRT, funcaoTratadora) == SIG_ERR) {
        // erro ao instalar a rotina de atendimento do sinal SIGINT
    }
}
