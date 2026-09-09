#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        perror("Missing argument (the sentence)");
        exit(1);
    }

    raise(SIGSTOP); // Waits for the signal to start

    while (1) {
        printf("%s\n", argv[1]);
        fflush(stdout); // Garantir que vá printar no mesmo instante
        usleep(200000); 
    }

    return 0;
}
