/*
    4) Modifique o programa anterior para que o filho execute um programa elaborado
por você, que mande imprimir uma mensagem qualquer no vídeo, por exemplo,
“alo mundo”. Em seguida altere o programa do item 4 para o filho executar o
programa echo da shell.
*/

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int main() {
    int mypid, pid, status;

    pid = fork();
    if (pid != 0) {
        // Pai
        waitpid(-1, &status, 0);
    }
    else {
        // Filho

        // Parte 1
        char* args[] = {"./helloWorld", NULL};
        execvp(args[0], args);

        // Parte 2
        // char* args[] = {"echo", "Oi mundo", NULL};
        // execvp(args[0], args);

        exit(3);
    }
    return 0;
}