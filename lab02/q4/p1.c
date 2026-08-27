#include "lab02-4-struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Uso: %s <shmid>\n", argv[0]);
        exit(1);
    }
    
    int segmento = atoi(argv[1]);
    Dado *dado = (Dado*)shmat(segmento, 0, 0);
    if (dado == (void*)-1) {
        puts("Erro ao dar attach");
        exit(1);
    }

    // Sleep de 1 a 7s
    srand(getpid());
    int wait_time = rand() % 7 + 1;
    sleep(wait_time);

    dado->valor = rand() % 100 + 1;
    dado->seq++;

    printf("Processo %s terminou com valor %02d após %ds\n", argv[0], dado->valor, wait_time);
    
    shmdt(dado);
    exit(0);
}
