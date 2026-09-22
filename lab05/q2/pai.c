#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "dados.h"

#define OPENMODE (O_RDONLY | O_NONBLOCK)

int main()
{    
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
    puts("[PAI] Abrindo a FIFO para leitura...");
    if ((fifo_pt = open(FIFO, OPENMODE)) < 0)
    {
        perror("Erro ao abrir a FIFO");
        return -2;
    }
    
    pid_t pids[QTD_FILHOS];
    for (int i = 0; i < QTD_FILHOS; i++)
    {
        if ((pids[i] = fork()) < 0)
        {
            perror("Erro ao criar um processo filho");
            exit(-3);
        }
        if (pids[i] == 0)
        {
            const char* args[3] = {"./filhos", "bom\n", NULL};
            execv("./filhos", (char*const*)args);
            perror("Erro ao executar o arquivo");
            exit(-4);
        }
    }

    for (int i = 0; i < QTD_FILHOS; i++) waitpid(pids[i], NULL, 0);

    char chLeitura;
    printf("Frases recebidas via FIFO:\n");
    while (read(fifo_pt, &chLeitura, sizeof(chLeitura)) > 0)
    {
        putchar(chLeitura);
    }

    close(fifo_pt);
    return 0;
}