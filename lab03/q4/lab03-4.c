#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>

#define QTD_FILHOS 2
#define QTD_TROCAS 10
#define EVER ;;

int main(void)  {
    pid_t pids[QTD_FILHOS]; // Processos filhos

    for (int i = 0; i < QTD_FILHOS; i++) {
        if ((pids[i] = fork()) < 0) {
            puts("Erro ao criar o filho");
            exit(3);
        }

        else if (pids[i] == 0) for(EVER); // Deixa filho rodando

        else kill(pids[i], SIGSTOP);
    }

    // Alternando SIGSTOP e SIGCONT
    for (int j = 0; j < QTD_TROCAS + 1; j++) {
        int i = j % QTD_FILHOS;
        sleep(1);
        printf("[%d] Acordando filho %d\n", j, pids[i]);
        kill(pids[i], SIGCONT);
        sleep(2);
        printf("[%d] Dormindo o filho %d\n", j, pids[i]);
        kill(pids[i], SIGSTOP);
    }

    for (int i = 0; i < QTD_FILHOS; i++) {
        sleep(1);
        printf("Matando o processo %d\n", pids[i]);
        sleep(1);
        kill(pids[i], SIGKILL);
    }

    return 0;
}
