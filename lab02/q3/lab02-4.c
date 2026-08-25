#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAM 10
#define qtdFilhos 4

int main() {
    int segmento;
    int* vetor = NULL;

    segmento = shmget(IPC_PRIVATE, sizeof(int) * TAM, IPC_CREAT | S_IRUSR | S_IWUSR);
    if (segmento == -1)
    {
        puts("Erro ao alocar memória");
        return 0;
    }

    vetor = (int*)shmat(segmento, 0, 0);
    if (vetor == (void*)-1)
    {
        puts("Erro ao dar attach");
        return 0;
    }

    printf("[");
    for (int i = 0; i < TAM; i++)
    {
        vetor[i] = rand() % 100;
        printf("%d ", vetor[i]);
    }
    printf("]\n");

    int valorBusca;
    printf("Digite o valor a buscar: ");
    scanf("%d", &valorBusca);

    int tamAreaVetor = (TAM + qtdFilhos - 1) / qtdFilhos; // Arredondar para cima a divisão
    pid_t pids[qtdFilhos];

    for (int i = 0; i < qtdFilhos; i++)
    {
        if ((pids[i] = fork()) < 0)
        {
            puts("Erro ao criar o filho");
            return 0;
        }
        else if (pids[i] == 0)
        {
            for (int pos = 0; pos < tamAreaVetor; pos++)
            {
                if (pos + i*tamAreaVetor > TAM) break;
                if (*(vetor + pos + i*tamAreaVetor) == valorBusca)
                    printf("Valor encontrado em %d\n", pos + i * tamAreaVetor);
            }
            exit(1);
        }
    }

    for (int i = 0; i < qtdFilhos; i++)
        waitpid(pids[i], NULL, 0);

    // libera a memória compartilhada do processo
    shmdt(vetor);

    // libera a memória compartilhada
    shmctl (segmento, IPC_RMID, 0);
    
    return 0;
}