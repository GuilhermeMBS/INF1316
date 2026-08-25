#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char msgDia[101];
    printf("Digite a mensagem do dia (até 100 caractéres): ");
    scanf("%s", msgDia);

    int segmento;
    char* memoria = NULL;

    segmento = shmget(8752, sizeof(msgDia), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (segmento == -1)
    {
        puts("Erro ao alocar memória");
        return 0;
    }

    memoria = (char*) shmat(segmento, 0, 0);
    if (memoria == (void*)-1)
    {
        puts("Erro ao attach memória");
        return 0;
    }

    strcpy(memoria, msgDia);

    // libera a memória compartilhada do processo
    shmdt(memoria);

    return 0;
}