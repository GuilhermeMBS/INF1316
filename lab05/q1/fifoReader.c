#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <fcntl.h>

#define FIFOMODE (S_IRUSR | S_IWUSR)
#define OPENMODE (O_RDONLY | O_NONBLOCK)

#define FIFO "minhaFifo"

int main()
{
    int fifo;
    char chLeitura;

    if (access(FIFO, F_OK) == -1)
    {
        if (mkfifo(FIFO, FIFOMODE) != 0)
        {
            puts("Erro ao criar a FIFO");
            return -1;
        }
    }

    puts("Abrindo a FIFO");
    if ((fifo = open(FIFO, OPENMODE)) < 0)
    {
        puts("Erro ao abrir a FIFO");
        return -2;
    }

    while (1)
    {
        while (read(fifo, &chLeitura, sizeof(chLeitura)) > 0)
        {
            putchar(chLeitura);
        }
    }

    return 0;
}