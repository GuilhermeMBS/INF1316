#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int order_vector(int* pt, int size) {
    char has_changes = 1;

    while (has_changes) {
        has_changes = 0;
        for (int i = 0; i < size - 1; i++) {
            if (*(pt + i) > *(pt + i + 1)) {
                int tmp = *(pt + i);
                *(pt + i) = *(pt + i + 1);
                *(pt + i + 1) = tmp;
                has_changes = 1;
            }
        }
    }
    
    return 0;
}

void print_vector(int* pt, int size) {
    printf("Vector:");
    for (int i = 0; i < size; i++, pt++) printf(" %d", *pt);
    printf("\n");

    return;
}

int main(void) {
    int mypid, pid, status;
    int v[10] = {10, 25, 46, 23, 20, 29, 72, 5, 60, 90};

    print_vector(v, 10);

    pid = fork();

    if (pid!=0) { //Pai
        mypid = getpid();
        // printf("PID do pai: %d\n", mypid);
        waitpid(-1, &status, 0);

        print_vector(v, 10);
    }

    else { //Filho
        (order_vector(v, 10) == 0) ? printf("Ordering successfull\n") : printf("Error while erdering\n");
        print_vector(v, 10);

        pid = getpid();
        // printf("PID do filho: %d\n", pid);

        exit(3);
    }

    return 0;
}
