#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void childhandler(int signo);
int delay;

int main (int argc, char *argv[]) {
    pid_t pid;
    signal(SIGCHLD, childhandler);

    // Error
    if ((pid = fork()) < 0) {
        fprintf(stderr, "Erro ao criar filho\n");
        exit(-1);
    }
    // Child
    if (pid == 0) {
        char* args[] = {argv[2], NULL};
        execve(argv[2], args, 0); /* ou sleep(3);*/
    }
    // Parent
    else {
        sscanf(argv[1], "%d", &delay); /* read delay from command line */
        sleep(delay);
        printf("Program %s exceeded limit of %d seconds!\n", argv[2], delay);
        kill(pid, SIGKILL);

        sleep(1); /* necessary for SIGCHLD to arrive */
    }

    return 0;
}

/* Executed if child dies before parent */
void childhandler(int signo) {
    int status;

    pid_t pid = wait(&status);
    printf("Child %d terminated within %d seconds with status %d.\n", pid, delay, status);

    exit(0);
}
