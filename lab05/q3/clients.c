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

    fifoR_pt = abreFifo(FIFO_Server2Client, 0);
    if (fifoR_pt < 0) return fifoR_pt;

    fifoW_pt = abreFifo(FIFO_Client2Server, 1);
    if (fifoW_pt < 0) return fifoW_pt;

    char fraseEscrita[TAM_FRASE];
    while (1)
    {
        printf("Digite a frase (de até %d char) que será processada: ", TAM_FRASE - 1);
        fgets(fraseEscrita, sizeof(fraseEscrita), stdin);
        write(fifoW_pt, fraseEscrita, strlen(fraseEscrita));

        read(fifoR_pt, fraseEscrita, sizeof(fraseEscrita));
        printf("Frase processada: %s", fraseEscrita);
    }

    return 0;
}