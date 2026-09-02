#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define AUTO 1


void tratador_sinais(int sinal) {
    switch (sinal) {
        case SIGINT:
            printf("\n[SIGINT %d] Você pressionou Ctrl-C. Processo continua ativo.\n", sinal);
            break;

        case SIGQUIT:
            printf("\n[SIGQUIT %d] Você pressionou Ctrl-\\. Encerrando de forma limpa...\n", sinal);
            exit(0);

        default:
            printf("\nSinal %d recebido sem ação definida.\n", sinal);
            break;
    }
}

void test_cmds() {
    sleep(2);
    raise(SIGINT);
    raise(SIGINT);
    sleep(2);
    raise(SIGINT);
    sleep(3);
    raise(SIGQUIT);
}

void get_signal() {
    while(1) pause();
}

int main(void) {
    // Ctrl-C
    if (signal(SIGINT, tratador_sinais) == SIG_ERR) {
        perror("Erro ao registrar SIGINT");
        exit(1);
    }

    // Ctrl-\ Signal
    if (signal(SIGQUIT, tratador_sinais) == SIG_ERR) {
        perror("Erro ao registrar SIGQUIT");
        exit(1);
    }

    printf("Tratadores ativos.\n");
    printf(" - Pressione Ctrl-C para testar a captura.\n");
    printf(" - Pressione Ctrl-\\ para encerrar o programa.\n");

    (AUTO) ? test_cmds() : get_signal();

    return 0;
}
