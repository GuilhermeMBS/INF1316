#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define MAXFILA 8
#define MAXPROD 64
#define TIMEPROD 1
#define TIMECONS 2


static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t can_produce = PTHREAD_COND_INITIALIZER;

typedef struct fila {
    int qtd;
    int valores[MAXFILA];
    int start;
    int end;
} Fila_t;

Fila_t estoque;


void produtor()
{
    int produzido = 0;
    while (produzido < MAXPROD) {
        produzido
        if(estoque.qtd == MAXFILA) { // full
            pthread_cond_wait(&can_produce, &mutex);
        }
    }
}


void consumidor()
{
    int consumido = 0;
    while (consumido < MAXPROD) {
        // Coisa
    }
}


int main()
{
    estoque.qtd = 0; estoque.start = 0; estoque.end = 0;

    pthread_t tidConsumidor, tidProdutor;
    pthread_create(&tidConsumidor, NULL, consumidor);
    pthread_create(&tidProdutor, NULL, produtor);

    pthread_join(tidConsumidor,NULL);
    pthread_join(tidProdutor,NULL);

    pthread_exit(NULL);
    pthread_mutex_destroy (&mutex);

    return 0;
}