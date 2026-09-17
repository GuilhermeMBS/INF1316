#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    int fd[2];

    if (pipe(fd) < 0) {
        puts("Erro ao abrir o pipe");
        exit(1);
    }

    pid_t pids[2];

    if ((pids[0] = fork()) < 0) {
        puts("Erro ao criar um filho");
        exit(2);
    }

    else if (pids[0] == 0) {
        // Filho -> Utilizar ps para escrever
        close(fd[0]); // Fechar a leitura da pipe

        // Utilizar o write da pipe como o out padrão
        dup2(fd[1], 1);

        close(fd[1]); // Evitar problemas

        execlp("ps", "ps", NULL);

        perror("Erro ao tentar executar ps");
        exit(3);
    }

    if ((pids[1] = fork()) < 0) {
        puts("Erro ao criar um filho");
        exit(2);
    }

    else if (pids[1] == 0) {
        // Filho -> Utilizar wc para printar
        close(fd[1]); // Fechar a escrita da pipe

        // Utilizar o read da pipe como in padrão
        dup2(fd[0], 0);

        close(fd[0]); // Evitar problemas

        execlp("wc", "wc", NULL);

        perror("Erro ao tentar executar wc");
        exit(4);
    }

    close(fd[0]);
    close(fd[1]);

    for (int i = 0; i < 2; i++) waitpid(pids[i], NULL, 0);
    
    return 0;
}
