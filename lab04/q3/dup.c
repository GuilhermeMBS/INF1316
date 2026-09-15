#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd[2];
    if (pipe(fd) < 0)
    {
        puts("Erro ao abrir o pipe");
        exit(1);
    }
    
    return 0;
}