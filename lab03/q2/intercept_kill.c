#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void meu_handler_kill(int sinal) {
    printf("Tentando tratar o SIGKILL (%d)... Este texto nunca vai aparecer!\n", sinal);
}

int main(void) {
    // Tenta interceptar o sinal SIGKILL (sinal 9)
    if (signal(SIGKILL, meu_handler_kill) == SIG_ERR) {
        perror("Erro ao registrar tratador para SIGKILL");
    } else {
        printf("Conseguiu registrar!\n");
    }

    return 0;
}
