/*
* 1) Execute o programa “ctrl-c.c”.
* Digite Ctrl-C e Ctrl-\. Analise o resultado.
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#define automatico 1

void funcaoTratadora (int sinal) {
    // tratamento do(s) sinal(is)
    printf("Sinal: %d\n", sinal);
    switch (sinal)
    {
        // Ctrl-C
        case SIGINT:
            puts("Ctrl-C evitado\n");
            break;
        // Ctrl-\ ()
        default:
            puts("Terminando a execução\n");
            exit(0);
    }
}

int main (void) {
    // captura Ctrl-C (SIGINT)
    if (signal(SIGINT, funcaoTratadora) == SIG_ERR) {
        // erro ao instalar a rotina de atendimento do sinal SIGINT
        puts("Erro ao alocar o handler do signal\n");
        exit(1);
    }
    // captura Ctrl-\ (SIGABRT; SIGQUIT - codespaces)
    else if (signal(SIGQUIT, funcaoTratadora) == SIG_ERR) {
        // erro ao instalar a rotina de atendimento do sinal SIGQUIT
        puts("Erro ao alocar o handler do signal\n");
        exit(1);
    }
    
    #ifdef automatico
        // Teste automático
        sleep(2);
        raise(SIGINT);
        sleep(2);
        raise(SIGQUIT);
    #else
        // Teste com o terminal
        for(;;);
    #endif

    
}
