#include "lab02-4-struct.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


int main() {
    int segmento1, segmento2;
    Dado *valor1, *valor2;

    segmento1 = shmget(IPC_PRIVATE, sizeof(Dado), IPC_CREAT | S_IRUSR | S_IWUSR);
    segmento2 = shmget(IPC_PRIVATE, sizeof(Dado), IPC_CREAT | S_IRUSR | S_IWUSR);
    if ((segmento1 == -1) || (segmento2 == -1)) {
        puts("Erro ao alocar memória");
        exit(1);
    }

    valor1 = (Dado*)shmat(segmento1, 0, 0);
    valor2 = (Dado*)shmat(segmento2, 0, 0);
    if ((valor1 == (void*)-1) || (valor2 == (void*)-1)) {
        puts("Erro ao dar attach");
        exit(1);
    }

    valor1->seq = 0;
    valor2->seq = 0;

    pid_t id1, id2;

    if ((id1 = fork()) < 0) {
        puts("Erro ao criar processo filho");
        exit(-2);
    }
    else if (id1 == 0) {
        char shmid[30];
        sprintf(shmid, "%d", segmento1);
        execl("./p1", "p1", shmid, NULL);

        exit(2);
    }

    if ((id2 = fork()) < 0) {
        puts("Erro ao criar processo filho");
        exit(-2);
    }
    else if (id2 == 0) {
        char shmid[30];
        sprintf(shmid, "%d", segmento2);
        execl("./p1", "p2", shmid, NULL);
        
        exit(2);
    }

    while(valor1->seq == 0 || valor2->seq == 0) sleep(1);

    printf("A multiplicação é dada por: %d * %d = %d\n", valor1->valor, valor2->valor, valor1->valor * valor2->valor);

    // Aguarda término dos processos para limpeza da tabela do Kernel
    waitpid(id1, NULL, 0);
    waitpid(id2, NULL, 0);

    // libera a memória compartilhada
    shmdt((void*)valor1);
    shmdt((void*)valor2);
    shmctl(segmento1, IPC_RMID, 0);
    shmctl(segmento2, IPC_RMID, 0);
    
    return 0;
}
