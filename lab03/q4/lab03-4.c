#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>

#define QTD_FILHOS 2
#define QTD_TROCAS 10

int main(void) 
{
    // Criando os processos filhos
    pid_t pids[QTD_FILHOS];
    for (int i = 0; i < QTD_FILHOS; i++)
    {
        if ((pids[i] = fork()) < 0)
        {
            puts("Erro ao criar o filho");
            exit(3);
        }
        else if (pids[i] == 0)
        {
            // Filho
            for(;;);
        }
        else 
        {
            kill(pids[i], SIGSTOP);
        }
    }

    // Alternando SIGSTOP e SIGCONT
    for (int j = 0; j < QTD_TROCAS; j++)
    {
        for (int i = 0; i < QTD_FILHOS; i++)
        {
            printf("Acordando filho %d\n", pids[i]);
            kill(pids[i], SIGCONT);
            sleep(2);
            printf("Dormindo o filho %d\n", pids[i]);
            kill(pids[i], SIGSTOP);
            sleep(1);
        }
    }

    for (int i = 0; i < QTD_FILHOS; i++)
    {
        printf("Matando o processo %d\n", pids[i]);
        kill(pids[i], SIGKILL);
    }

    return 0;
}