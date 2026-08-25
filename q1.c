#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>

#define ROWS 3
#define COLUMNS 3
#define MSIZE (sizeof(int)*ROWS*COLUMNS)
#define MEADD(m, i, j) (m)


int fill_matrices(int* m1, int* m2) {
    printf("First Matrix\n";)
    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column < COLUMNS; column++) {
            scanf("%d", MEADD(m1, i, j)); 
            printf("%d ", MEADD(m1, i, j));
        }
        printf("\n");
    }

    printf("\nSecond Matrix\n";)
    for (int row = 0; row < ROWS; row++) {
        for (int column = 0; column < COLUMNS; column++) {
            MEL(m2, i, j);
        }
    }

    return 0;
}

int main (int argc, char *argv[])
{
    int mmem_01, mmem_02, mmem_03;
    int* m1, m2;
    
    // int id, pid, status;

    mmem_01 = shmget (IPC_PRIVATE, MSIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    mmem_02 = shmget (IPC_PRIVATE, MSIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    mmem_03 = shmget (IPC_PRIVATE, MSIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    m1 = (int*) shmat (segmento, 0, 0);
    m2 = (int*) shmat (segmento, 0, 0);
    if (m1 == -1 | m2 == -1) exit(1);

    fill_matrices(m1, m2);

    for (int i = 0; i < ROWS; i++) {
        if ((id = fork()) < 0)
            {
                puts ("Erro na criação do novo processo");
                exit (-2);
            }
        else if (id == 0)
            {
                *p += 5;
                printf ("Processo filho = %d\n", *p);
            }
        else
            {
                pid = wait (&status);
                *p += 10;
                printf ("Processo pai = %d\n", *p);
            }
    }

    // libera a memória compartilhada do processo
    shmdt (p);

    // libera a memória compartilhada
    shmctl (segmento, IPC_RMID, 0);

    return 0;
}