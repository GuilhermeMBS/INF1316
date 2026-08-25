#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>


#define LINHAS 4
#define COLUNAS 3

int main() {
    int segmento1, segmento2, segmento3, id, pid, status, *mat1, *mat2, *mat3;
    // aloca a memória compartilhada
    segmento1 = shmget (IPC_PRIVATE, (sizeof(int) * LINHAS * COLUNAS), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    segmento2 = shmget (IPC_PRIVATE, (sizeof(int) * LINHAS * COLUNAS), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    segmento3 = shmget (IPC_PRIVATE, (sizeof(int) * LINHAS * COLUNAS), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

    // associa a memória compartilhada ao processo
    mat1 = (int *) shmat (segmento1, 0, 0); // comparar o retorno com -1
    printf("Matriz 1:\n");
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            *(mat1+i*COLUNAS + j) = COLUNAS*i + j + 1;
            printf("%02d ", *(mat1+i*COLUNAS + j));
        }
        printf("\n");
    }
    printf("\n");

    mat2 = (int *) shmat (segmento2, 0, 0); // comparar o retorno com -1
    printf("Matriz 2:\n");
    for (int i = 0; i < LINHAS; i++)
    {
        for (int j = 0; j < COLUNAS; j++)
        {
            *(mat2+i*COLUNAS + j) = 2*(COLUNAS*i + j + 1);
            printf("%02d ", *(mat2+i*COLUNAS + j));
        }
        printf("\n");
    }
    printf("\n");

    mat3 = (int *) shmat (segmento3, 0, 0); // comparar o retorno com -1
    for (int i = 0; i < LINHAS; i++)
        for (int j = 0; j < COLUNAS; j++)
            *(mat3 + i*COLUNAS + j) = 0;

    printf("Matriz 3 (soma das anteriores):\n");
    for (int linha = 0; linha < LINHAS; linha++)
    {
        if ((id = fork()) < 0)
        {
            puts("Erro");
            exit(-2);
        }
        else if (id == 0)
        {
            for (int j = 0; j < COLUNAS; j++)
                *(mat3 + linha*COLUNAS + j) = *(mat2 + linha*COLUNAS + j) + *(mat1 + linha*COLUNAS + j);
            exit(2);
        }
        else
        {
            pid = wait(&status);
            for (int j = 0; j < COLUNAS; j++)
            {
                printf("%02d ", *(mat3 + linha*COLUNAS + j));
                if (j == 2) printf("\n");
            }
        }
    }

    // libera a memória compartilhada do processo
    shmdt(mat1);
    shmdt(mat2);
    shmdt(mat3);

    // libera a memória compartilhada
    shmctl (segmento1, IPC_RMID, 0);
    shmctl (segmento2, IPC_RMID, 0);
    shmctl (segmento3, IPC_RMID, 0);
    return 0;
}