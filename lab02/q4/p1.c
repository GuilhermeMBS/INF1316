#include "struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s <shmid>\n", argv[0]);
        return 1;
    }
    
    int segmento = atoi(argv[1]);
    Dado *valor = (Dado *)shmat(segmento, 0, 0);
    if (valor == (void*)-1)
    {
        puts("Erro ao dar attach");
        return 1;
    }

    // Sleep de 1 a 7s
    srand(getpid());

    sleep(rand() % 7 + 1);
    valor->valor = rand() % 100 + 1;
    valor->seq++;

    printf("Processo %s terminou\n", argv[0]);
    
    shmdt(valor);
    exit(0);
}