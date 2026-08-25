#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


int main() {
    int segmento;
    char* memoria = NULL;

    segmento = shmget(8752, sizeof(char) * 101, S_IRUSR | S_IWUSR);
    if (segmento == -1)
    {
        puts("Erro ao alocar memória");
        return 0;
    }

    memoria = (char*)shmat(segmento, 0, 0);
    if (memoria == (void*)-1)
    {
        puts("Erro ao dar attach");
        return 0;
    }

    printf("Msg armazenada: %s\n", memoria);

    // libera a memória compartilhada do processo
    shmdt(memoria);

    // libera a memória compartilhada
    shmctl (segmento, IPC_RMID, 0);

    return 0;
}