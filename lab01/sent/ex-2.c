/*
    2) Agora, usando a mesma estrutura de processos pai e filho, declare uma
variável visível ao pai e ao filho, no pai inicialize a variável com1 e imprima seu
valor antes do fork(). No filho, altere o valor da variável para 5 e imprima o seu
valor antes do exit(). Agora, no pai, imprima novamente o valor da variável após o
filho ter alterado a variável - após a waitpid(). Justifique os resultados obtidos.
*/

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int mypid, pid, status;
    
    int var = 1;
    printf("Var: %d\n", var);

    pid = fork();
    if (pid != 0) {
        // Pai
        mypid = getpid();
        printf("PID pai: %d\n", mypid);
        waitpid(-1, &status, 0);
        printf("Var: %d\n", var);
    }
    else {
        // Filho
        mypid = getpid();
        printf("PID filho: %d\n", mypid);
        var = 5;
        printf("Var: %d\n", var);
        exit(3);
    }
    return 0;
}