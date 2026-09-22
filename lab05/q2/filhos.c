#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include "dados.h"

#define OPENMODE (O_WRONLY | O_NONBLOCK)

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        perror("It must have a string. Like: ./filhos olaBomDia");
        exit(-3);
    }

    if (access(FIFO, F_OK) == -1)
    {
        puts("Criando a FIFO...");
        if (mkfifo(FIFO, FIFOMODE) != 0)
        {
            perror("Erro ao criar a FIFO");
            return -1;
        }
    }
    
    int fifo_pt;
    puts("[FILHO] Abrindo a FIFO para escrita...");
    if ((fifo_pt = open(FIFO, OPENMODE)) < 0)
    {
        perror("Erro ao abrir a FIFO");
        return -2;
    }
    
    write(fifo_pt, argv[1], strlen(argv[1]));

    close(fifo_pt);

    return 0;
}