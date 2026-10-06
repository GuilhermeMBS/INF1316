#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 5
#define PULO_MAXIMO 100
#define DESCANSO_MAXIMO 1
#define DISTANCIA_PARA_CORRER 100


static int classificacao = 1;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int cont = 0;

void *Correr(void *sapo)
{
    int pulos = 0;
    int distanciaJaCorrida = 0;
    int* sapo_num = (int *) sapo;

    while (distanciaJaCorrida <= DISTANCIA_PARA_CORRER) {
        int pulo = rand() % PULO_MAXIMO;
        distanciaJaCorrida += pulo;
        pulos++;

        printf("Sapo %d pulou\n", *sapo_num);

        int descanso = rand() % DESCANSO_MAXIMO;
        sleep(descanso);
    }

    pthread_mutex_lock(&lock);
    
    printf("Sapo %d chegou na posicao %d com %d pulos\n", *sapo_num, classificacao, pulos);
    cont++;
    classificacao++;

    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int main()
{
    classificacao =1;
    pthread_t threads[NUM_THREADS];
    int t;

    printf("Corrida iniciada ... \n");

    int nums[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) nums[i] = i;

    for(t=0;t < NUM_THREADS;t++) pthread_create(&threads[t], NULL, Correr, (void *) &nums[t]);
    for(t=0;t < NUM_THREADS; t++) pthread_join(threads[t],NULL);

    printf("\n Acabou!!\n");
    pthread_exit(NULL);
    return 0;
}