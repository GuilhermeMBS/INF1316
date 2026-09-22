#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <signal.h> // Para tratar o CTRL-C

#include "dados.h"

#define OPENMODE (O_RDONLY | O_NONBLOCK)

int fifo_pt;

void encerrar(int sinal)
{
    switch (sinal)
    {
        case SIGINT:
            puts("\nFechando a FIFO...");
            close(fifo_pt);
            exit(0);

        default:
            puts("Signal não reconhecido");
            break;
    }
}

int main()
{
    if (signal(SIGINT, encerrar) == SIG_ERR) {
        perror("Erro ao registrar SIGINT");
        exit(-3);
    }

    
    if (access(FIFO, F_OK) == -1)
    {
        puts("Criando a FIFO...");
        if (mkfifo(FIFO, FIFOMODE) != 0)
        {
            puts("Erro ao criar a FIFO");
            return -1;
        }
    }
    
    puts("Abrindo a FIFO...");
    if ((fifo_pt = open(FIFO, OPENMODE)) < 0)
    {
        puts("Erro ao abrir a FIFO");
        return -2;
    }

    char chLeitura;

    while (1)
    {
        while (read(fifo_pt, &chLeitura, sizeof(chLeitura)) > 0)
        {
            putchar(chLeitura);
        }
    }

    return 0;
}