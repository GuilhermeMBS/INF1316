#include <stdio.h>
#include <stdlib.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define TAM 10
#define qtdFilhos 4
#define DEBUG 0     // Tests if they are parallel processes


int main() {
    int segmento;
    int* vetor = NULL;

    segmento = shmget(IPC_PRIVATE, sizeof(int) * (TAM + 1), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (segmento == -1) {
        puts("Erro ao alocar memória");
        exit(1);
    }

    vetor = (int*)shmat(segmento, 0, 0);
    if (vetor == (void*)-1) {
        puts("Erro ao dar attach");
        exit(1);
    }

    vetor[TAM] = 0; // Flag for the desired value (if it was found)
    printf("[ ");
    for (int i = 0; i < TAM; i++) {
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
        if ((pids[i] = fork()) < 0) {
            puts("Erro ao criar o filho");
            exit(-2);
        }
        else if (pids[i] == 0) {
            if (DEBUG) {
                printf("Process %d beggins...\n", i);
                sleep(1);
            }

            for (int pos = 0; pos < tamAreaVetor; pos++) {
                if (pos + i*tamAreaVetor >= TAM) break;

                if (*(vetor + pos + i*tamAreaVetor) == valorBusca) {
                    vetor[TAM] = 1;
                    printf("Valor encontrado no index %d\n", pos + i * tamAreaVetor);
                }
            }

            shmdt(vetor);

            if (DEBUG) printf("Process %d ends...\n", i);           
            exit(2);
        }
    }

    for (int i = 0; i < qtdFilhos; i++) waitpid(pids[i], NULL, 0);
    if (vetor[TAM] == 0) printf("Valor ausente da lista!\n");

    // libera a memória compaprtilhada
    shmdt(vetor);
    shmctl (segmento, IPC_RMID, 0);
    
    return 0;
}
