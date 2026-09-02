#include <stdio.h>
#include <unistd.h>

int main (void) {
    fprintf(stdout, "Indo dormir...\n");
    sleep(5);
    fprintf(stdout, "Acordei!\n");
    
    return 0;
}