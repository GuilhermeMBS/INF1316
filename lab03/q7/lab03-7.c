#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define QTDPROCESS 3

pid_t pids[QTDPROCESS];

void handle_sig(int signal) {
    if (signal == SIGINT) {
        puts("\nEnding child process...");
        for (int i = 0; i < QTDPROCESS; i++) kill(pids[i], SIGKILL);
        puts("Ending parent process...");
        exit(0);
    }

    return;
}


int main(void) {
    if (signal(SIGINT, handle_sig) == SIG_ERR) {
        perror("Error while starting a call.");
        exit(1);
    }

    char* sentences[] = {
        "[FILHO 1] Ola, sou o filho 1",
        "[FILHO 2] Teste do filho 2",
        "[FILHO 3] Agora o terceiro"
    };

    for (int i = 0; i < QTDPROCESS; i++) {
        pids[i] = fork();

        // Erro
        if (pids[i] < 0) {
            perror("Fork failed");
            exit(1);
        }
        
        // Filho
        else if (pids[i] == 0) {
            // Implementar o exec dos outros códigos
            char* args[] = {"./io1", sentences[i], NULL};
            execve("./io1", args, NULL);
            perror("Erro no execve");
            exit(1);
        }

        // Pai
        else kill(pids[i], SIGSTOP);
    }
    
    unsigned int delay;
    int atual = 0; // Processo que assume o controle3

    while (1) {
        kill(pids[atual], SIGCONT);
        delay = (atual == 0) ? 1 : 2; // 1 sec para o primeiro processo, e 2 sec para os demais 
        sleep(delay);
        kill(pids[atual], SIGSTOP);
        atual = (atual + 1) % QTDPROCESS;
    }

    return 0;
}
