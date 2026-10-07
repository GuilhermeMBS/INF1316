#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAXFILA 8
#define TOTAL_ELEMENTOS 64


int fila[MAXFILA];
int in = 0;     // Índice de inserção (produtor)
int out = 0;    // Índice de remoção (consumidor)
int count = 0;  // Quantidade atual de elementos na fila

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_produtor = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_consumidor = PTHREAD_COND_INITIALIZER;

void* produtor(void* arg) {
    for (int i = 0; i < TOTAL_ELEMENTOS; i++) {
        int item = rand() % 100; // Número pseudo-aleatório

        pthread_mutex_lock(&mutex); // Bloqueia acesso à fila

        // Se a fila estiver cheia, o produtor dorme e libera o mutex
        while (count == MAXFILA) {
            pthread_cond_wait(&cond_produtor, &mutex);
        }

        // Insere na fila circular
        fila[in] = item;
        printf("[PRODUTOR] Inseriu: %d \t(Pos: %d | Total na fila: %d)\n", item, in, count + 1);
        in = (in + 1) % MAXFILA;
        count++;

        // Avisa o consumidor que há um novo item na fila
        pthread_cond_signal(&cond_consumidor);
        
        pthread_mutex_unlock(&mutex); // Libera o acesso à fila

        sleep(1);
    }
    return NULL;
}

void* consumidor(void* arg) {
    for (int i = 0; i < TOTAL_ELEMENTOS; i++) {
        pthread_mutex_lock(&mutex); // Bloqueia acesso à fila

        // Se a fila estiver vazia, o consumidor dorme e libera o mutex
        while (count == 0) {
            pthread_cond_wait(&cond_consumidor, &mutex);
        }

        // Retira da fila circular
        int item = fila[out];
        printf("[CONSUMIDOR] Retirou: %d \t(Pos: %d | Total na fila: %d)\n", item, out, count - 1);
        out = (out + 1) % MAXFILA;
        count--;

        // Avisa o produtor que abriu espaço na fila
        pthread_cond_signal(&cond_produtor);
        
        pthread_mutex_unlock(&mutex); // Libera o acesso à fila

        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t thread_prod, thread_cons;
    srand(time(NULL));

    pthread_create(&thread_prod, NULL, produtor, NULL);
    pthread_create(&thread_cons, NULL, consumidor, NULL);

    pthread_join(thread_prod, NULL);
    pthread_join(thread_cons, NULL);

    printf("\nExecução finalizada. Total produzido/consumido: %d\n", TOTAL_ELEMENTOS);
    return 0;
}
