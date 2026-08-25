#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

void print_var(int var) {
    printf("var = %d\n", var);

    return;
}

int main(void) {
    int mypid, pid, status;
    int var = 1;

    print_var(var);

    pid = fork();

    if (pid!=0) { //Pai
        mypid = getpid();
        // printf("PID do pai: %d\n", mypid);
        waitpid(-1, &status, 0);

        print_var(var);
    }

    else { //Filho
        var = 5;

        print_var(var);

        pid = getpid();
        // printf("PID do filho: %d\n", pid);

        exit(3);
    }

    return 0;
}
