#include "struct.h"

#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct Dado Dado;

int main() {
    int segmento1, segmento2;
    Dado *valor1, *valor2;

    segmento1 = shmget(IPC_PRIVATE, sizeof(Dado), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (segmento1 == -1)
    {
        puts("Erro ao alocar memória");
        return 0;
    }

    valor1 = (Dado*)shmat(segmento1, 0, 0);
    if (valor1 == (void*)-1)
    {
        puts("Erro ao dar attach");
        return 0;
    }
    valor1->seq = 0;

    segmento2 = shmget(IPC_PRIVATE, sizeof(Dado), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (segmento2 == -1)
    {
        puts("Erro ao alocar memória");
        return 0;
    }

    valor2 = (Dado*)shmat(segmento2, 0, 0);
    if (valor2 == (void*)-1)
    {
        puts("Erro ao dar attach");
        return 0;
    }
    valor2->seq = 0;

    int id1, id2;

    if ((id1 = fork()) < 0)
    {
        puts("Erro ao criar processo filho");
        return 1;
    }
    else if (id1 == 0)
    {
        char shmid[30];
        sprintf(shmid, "%d", segmento1);
        execl("./p1", "p1", shmid, NULL);

        exit(0);
    }

    if ((id2 = fork()) < 0)
    {
        puts("Erro ao alocar memoria");
        return 1;
    }
    else if (id2 == 0)
    {
        char shmid[30];
        sprintf(shmid, "%d", segmento2);
        execl("./p1", "p2", shmid, NULL);
        
        exit(0);
    }

    while(valor1->seq == 0 || valor2->seq == 0)
        sleep(0.5);

    printf("A multiplicação dos valores %d * %d é: %d\n", valor1->valor, valor2->valor, valor1->valor * valor2->valor);

    // libera a memória compartilhada do processo
    shmdt(valor1); shmdt(valor2);

    // libera a memória compartilhada
    shmctl(segmento1, IPC_RMID, 0);
    shmctl(segmento2, IPC_RMID, 0);
    
    return 0;
}