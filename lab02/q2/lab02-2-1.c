#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define MSG_SIZE 4096

int main() {
    char msgDia[MSG_SIZE];
    fgets(msgDia, sizeof(msgDia), stdin);
    msgDia[strcspn(msgDia, "\n")] = '\0'; // Removes the '\n' at the end

    char* memoria = NULL;
    int segmento = shmget(8752, sizeof(msgDia), IPC_CREAT | S_IRUSR | S_IWUSR);
    
    if (segmento == -1) {
        puts("Erro ao alocar memória");
        exit(1);
    }

    memoria = (char*) shmat(segmento, 0, 0);
    if (memoria == (void*)-1) {
        puts("Erro no attach da memória");
        exit(1);
    }

    strcpy(memoria, msgDia);
    shmdt(memoria);

    return 0;
}
