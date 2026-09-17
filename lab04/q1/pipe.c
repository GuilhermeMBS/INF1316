#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAM_MSG 16


int main() {
    int fd[2];

    if (pipe(fd) < 0) {
        puts("Erro ao abrir a pipe");
        exit(1);
    }

    pid_t pid;
    if ((pid = fork()) < 0) {
        puts("Erro ao criar o filho");
        exit(2);
    }

    else if (pid == 0) {
        // Filho
        close(fd[0]);
        const char txt_TX[TAM_MSG] = "Ola mundo!";
        write(fd[1], txt_TX, sizeof txt_TX);
        printf("Foram escritos %d dados\n", (int)sizeof txt_TX);
        close(fd[1]);
    }

    else {
        // Pai
        close(fd[1]);
        char txt_RX[TAM_MSG];
        int qtd = read(fd[0], txt_RX, sizeof txt_RX);
        printf("Foram lidos %d dados\n", qtd);
        printf("%s\n", txt_RX);
        close(fd[0]);

        wait(NULL); // Aguarda o filho
    }

    return 0;
}
