#pragma once

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFOMODE (S_IRUSR | S_IWUSR)

#define OPENMODE_R (O_RDONLY)
#define OPENMODE_W (O_WRONLY)

#define TAM_FRASE 100
#define FIFO_Server2Client "FIFOW"
#define FIFO_Client2Server "FIFOR"

/**
 * @brief isWrite 1 -> Modo de escrita; 0 -> Modo de leitura
 */
int abreFifo(const char* nameFifo, char isWrite)
{
    int tmp;
    const char* tipo = (isWrite == 1) ? "escrita" : "leitura";
    if (access(nameFifo, F_OK) == -1)
    {
        printf("Criando a FIFO para %s...\n", tipo);
        if (mkfifo(nameFifo, FIFOMODE) != 0)
        {
            puts("Erro ao criar a FIFO");
            return -1;
        }
    }
    
    printf("Abrindo a FIFO para %s...\n", tipo);
    int mode = (isWrite == 1) ? OPENMODE_W : OPENMODE_R;
    if ((tmp = open(nameFifo, mode)) < 0)
    {
        puts("Erro ao abrir a FIFO");
        return -2;
    }

    return tmp;
}