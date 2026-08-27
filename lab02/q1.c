#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define ROWS 3
#define COLUMNS 3
#define MSIZE (sizeof(int)*(ROWS)*(COLUMNS))


static inline int* get_element_ptr(int *matrix, int i, int j) {
    return matrix + ((COLUMNS*i) + j );
}

static inline int* sum_elements(int* a_ij, int* b_ij, int* c_ij) {
    *c_ij = *a_ij + *b_ij;
    return 0;
}

int fill_matrices(int* m1, int* m2) {
    printf("First Matrix\n");
    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column < COLUMNS; column++) {
            scanf("%d", get_element_ptr(m1, row, column)); 
            printf("%d ", get_element_ptr(m1, row, column));
        }
        printf("\n");
    }

    printf("\nSecond Matrix\n");
    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column < COLUMNS; column++) {
            scanf("%d", get_element_ptr(m2, row, column)); 
            printf("%d ", get_element_ptr(m2, row, column));
        }
        printf("\n");
    }

    return 0;
}

int main (int argc, char *argv[])
{
    int mmem_01, mmem_02, mmem_03;
    int* m1, m2, m3;
    
    mmem_01 = shmget (IPC_PRIVATE, MSIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    mmem_02 = shmget (IPC_PRIVATE, MSIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    mmem_03 = shmget (IPC_PRIVATE, MSIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    m1 = (int*) shmat (mmem_01, 0, 0);
    m2 = (int*) shmat (mmem_02, 0, 0);
    m2 = (int*) shmat (mmem_03, 0, 0);
    if (m1 == -1 | m2 == -1) exit(1);

    fill_matrices(m1, m2);

    // Creates ROWS process to calculate the sum for each column
    for (int i = 0; i < ROWS; i++) {
        int id, pid, status;

        if ((id = fork()) < 0) {
            puts ("Fork Error");
            exit (-2);
        }
        else if (id == 0) {
            // sum_elements()
            // printf("Processo filho = %d\n", *p);
        }
        else {
            pid = wait (&status);
            // printf("Processo pai = %d\n", *p);
        }
    }

    // libera a memória compartilhada do processo
    shmdt (m1); shmdt(m2); shmdt(m3);

    // libera a memória compartilhada
    shmctl (mmem_01, IPC_RMID, 0);
    shmctl (mmem_02, IPC_RMID, 0);
    shmctl (mmem_03, IPC_RMID, 0);

    return 0;
}
