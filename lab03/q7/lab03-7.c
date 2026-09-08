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

#define QTDPROCESS 3

pid_t pids[QTDPROCESS];

void handle_sig(int signal) {
    if (signal == SIGINT)
    {
        puts("Ending child process...\n");
        for (int i = 0; i < QTDPROCESS; i++)
        {
            kill(pids[i], SIGKILL);
        }
        puts("Ending parent process...\n");
        eit(0);
    }
    return;
}


int main(void) {
    if (signal(SIGINT, handle_sig) == SIG_ERR) {
        perror("Error while starting a call.");
        exit(1);
    }

    pid_t pids[QTDPROCESS];

    for (int i =0; i < QTDPROCESS; i++)
    {
        if ((pids[i] = fork()) < 0) // Erro
        {
            perror("Error while forking the process");
        }
        else if (pids[i] = 0) // Filho
        {
            // Implementar o exec dos outros códigos
            // execv()
        }
        else // Pai
        {
            kill(pids[i], SIGSTOP);
        }
    }
    
    unsigned int delay;
    int atual = 0; // Processo que assume o controle3
    while (1)
    {
        kill(pids[atual], SIGCONT);
        delay = (atual == 0) ? 1 : 2; // 1 sec para o primeiro processo, e 2 sec para os demais 
        sleep(delay);
        atual = (atual + 1) % QTDPROCESS;
    }

    return 0;
}
