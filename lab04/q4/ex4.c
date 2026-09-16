/**
 * Faça um programa que cria dois processos leitores e um processo escritor
 * em uma mesma pipe. Faça o escritor dormir metade do tempo dos leitores,
 * e mostre como os leitores consomem os dados produzidos pelo escritor.
 * Obs: não force uma alternância controlada por SIGSTOP/SIGCONT
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define TOTAL_MESSAGES 10
#define WRITER_SLEEP_US 500000
#define READER_SLEEP_US 1000000


void reader_routine(int id, int read_fd) {
    int value;

    while (read(read_fd, &value, sizeof(int)) > 0) {
        printf("  [Reader %d | PID %d] Consumed: %02d\n", id, getpid(), value);
        fflush(stdout);
        usleep(READER_SLEEP_US);
    }

    close(read_fd);
    exit(0);
}


void writer_routine(int write_fd) {
    for (int i = 1; i <= TOTAL_MESSAGES; i++) {
        printf("[Writer | PID %d] Produced: %02d\n", getpid(), i);
        fflush(stdout);

        if (write(write_fd, &i, sizeof(int)) < 0) {
            perror("write failed");
            break;
        }

        usleep(WRITER_SLEEP_US);
    }

    close(write_fd);
    exit(0);
}


int main(void) {
    int fd[2];

    if (pipe(fd) < 0) {
        perror("Error: Pipe Failed");
        exit(1);
    }

    // Reader 1
    if (fork() == 0) {
        close(fd[1]);
        reader_routine(1, fd[0]);
    }

    // Reader 2
    if (fork() == 0) {
        close(fd[1]);
        reader_routine(2, fd[0]);
    }

    // Writer
    if (fork() == 0) {
        close(fd[0]);
        writer_routine(fd[1]);
    }

    close(fd[0]);
    close(fd[1]);

    for (int i = 0; i < 3; i++) wait(NULL);

    return 0;
}
