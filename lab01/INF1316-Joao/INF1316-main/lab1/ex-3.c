/*
    3) Use o programa anterior para ler e ordenar um vetor de 10 posições. O filho
ordena o vetor e o pai exibe os dados do vetor antes do fork() e depois do
waitpid(). Eles usarão o mesmo vetor na memória? Justifique.
*/

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int main() {
    int mypid, pid, status;
    
    int var[] = {23, 5, 10, 99, 80, 4, 45, 68, 50, 33};
    
    printf("Pai (antes do fork): {");
    for (int i = 0; i < SIZE; i++) printf("%d, ", var[i]);
    printf("}\n");

    pid = fork();
    if (pid != 0) {
        // Pai
        waitpid(-1, &status, 0);
        
        printf("Pai (depois do waitpid): {");
        for (int i = 0; i < SIZE; i++) printf("%d, ", var[i]);
        printf("}\n");
    }
    else {
        // Filho
        char has_changes = 1;
        while (has_changes) {
            has_changes = 0;
            for (int i = 0; i < SIZE - 1; i++) {
                if (var[i] > var[i+1]) {
                    has_changes = 1;
                    int tmp = var[i];
                    var[i] = var[i+1];
                    var[i+1] = tmp;
                }
            }
        }

        
        printf("Filho: {");
        for (int i = 0; i < SIZE; i++) printf("%d, ", var[i]);
        printf("}\n");

        exit(3);
    }
    return 0;
}