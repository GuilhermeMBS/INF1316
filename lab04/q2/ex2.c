/**
 * Faça um programa para redirecionar a entrada e a saída,
 * lendo os dados de um arquivo e gerando a saída em outro.
 * 
 * Comandos (Apagar o out.txt antes de executá-los):
 * $ gcc -Wall -o ex2 ex2.c && ./ex2
 * $ cat out.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    int fd_in, fd_out;

    fd_in = open("in.txt", O_RDONLY);
    if (fd_in < 0) {
        perror("Error while opening in.txt");
        exit(1);
    }

    fd_out = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        perror("Error while opening out.txt");
        close(fd_in);
        exit(1);
    }

    if (dup2(fd_in, STDIN_FILENO) < 0) {
        perror("Error in dup2 for stdin");
        exit(1);
    }

    if (dup2(fd_out, STDOUT_FILENO) < 0) {
        perror("Error in dup2 for stdout");
        exit(1);
    }

    close(fd_in);
    close(fd_out);

    char row[256];
    int count = 1;

    while (fgets(row, sizeof(row), stdin) != NULL) {
        printf("[Row %02d]: %s", count++, row);
    }

    return 0;
}
