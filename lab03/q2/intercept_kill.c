#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void handler_kill(int sinal) {
    printf("Tentando tratar o SIGKILL (%d)... Este texto nunca vai aparecer!\n", sinal);
}

int main(void) {
    // Handler tenta interceptar o sinal SIGKILL
    if (signal(SIGKILL, handler_kill) == SIG_ERR) {
        printf("Erro ao registrar tratador para SIGKILL!\n");
    } else {
        printf("Conseguimos interceptar o SIGKILL!\n");
    }

    return 0;
}
