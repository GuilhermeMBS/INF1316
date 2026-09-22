#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <signal.h> // Para tratar o CTRL-C

#include <string.h> // Para strlen
#include <ctype.h> // Para o toupper()

#include "dados.h"

int fifoW_pt, fifoR_pt;

void encerrar(int sinal)
{
    switch (sinal)
    {
        case SIGINT:
            puts("\nFechando as FIFOs...");
            close(fifoW_pt);
            close(fifoR_pt);
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

    fifoW_pt = abreFifo(FIFO_Server2Client, 1);
    if (fifoW_pt < 0) return fifoW_pt;

    fifoR_pt = abreFifo(FIFO_Client2Server, 0);
    if (fifoR_pt < 0) return fifoR_pt;

    char fraseLeitura[TAM_FRASE];
    while (1)
    {
        if (read(fifoR_pt, fraseLeitura, sizeof(fraseLeitura)) > 0)
        {
            for (int i = 0; i < strlen(fraseLeitura); i++)
            {
                fraseLeitura[i] = toupper(fraseLeitura[i]);
            }
            write(fifoW_pt, fraseLeitura, strlen(fraseLeitura));
        }
    }

    return 0;
}