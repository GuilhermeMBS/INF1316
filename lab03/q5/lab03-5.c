/**
 * 5) Faça um programa que leia 2 números e imprima o resultado das 4 operações básicas
 * sobre estes 2 números.
 * Verifique o que acontece se o 2º. número da entrada for 0 (zero)
 * Capture o sinal de erro de floating point (SIGFPE) e repita a experiência anterior
 */
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#define CAPTURE_SIG 0

#if CAPTURE_SIG
void handle_sig(int signal) {
    if (signal == SIGFPE) {
        puts("Floating point error captured!");
        exit(2);
    }
}
#endif

void print_operations(int x, int y) {
    printf("%d + %d = %d\n", x, y, x + y);
    printf("%d - %d = %d\n", x, y, x - y);
    printf("%d * %d = %d\n", x, y, x * y);
    printf("%d / %d = %d\n", x, y, x / y);
}

int main(void) {
    int x, y;

    puts("Input x and y: ");
    scanf("%d %d", &x, &y);

    #if CAPTURE_SIG
    if (signal(SIGFPE, handle_sig) == SIG_ERR) {
        perror("Floating Point Signal Capture Error");
        exit(1);
    }
    #endif

    puts("\n=== Manual Inputs ===");
    print_operations(x, y);
    puts("=====================");
    puts("\n=== Case With Zero ===");
    print_operations(x, 0);
    puts("======================\n");

    return 0;
}
