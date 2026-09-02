#include <stdio.h>
#include <unistd.h>

int main (void) {
    fprintf(stdout, "Indo dormir...\n");
    sleep(15);
    fprintf(stdout, "Acordei!\n");

    return 0;
}