/*
Escreva um programa log_gen.c que usa void gerar_operacoes(int n)
para que ele escreva 10.000 operações em um arquivo log.txt 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_KEY 999
#define MIN_KEY 0

typedef enum {
    READ, 
    WRITE
} OpType;

// Função que gera e exibe N operações aleatórias
void gerar_operacoes(int n) {
    FILE* file = fopen("log.txt", "w");
    if (file == NULL) {
        perror("Error while creating the log file");
    }

    // Inicializa o gerador de números aleatórios com o tempo atual
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        // Escolhe aleatoriamente entre READ (0) e WRITE (1)
        OpType op = rand() % 2;

        // Gera key aleatória no intervalo de 0 a 999
        int key = (rand() % (MAX_KEY - MIN_KEY + 1)) + MIN_KEY;
        if (op == READ) {
            // Formato pedido: R, key
            printf("R, %d\n", key);

            fprintf(file, "R %d\n", key);
        } else {
            // Gera um valor inteiro aleatório qualquer (ex: entre 10 e 5000)
            int value = (rand() % 4991) + 10;

            // Formato pedido: W, value, key
            printf("W, %d, %d\n", value, key);

            fprintf(file, "W %d %d\n", value, key);
        }
    }
    fclose(file);
}

int main() {
    gerar_operacoes(10000);
    return 0;
}