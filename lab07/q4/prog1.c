#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


#define FILE_NAME "shared_data.bin"
#define SHARED_SIZE 4096


int
main(void)
{
    int fd = open(FILE_NAME, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open prog1");
        exit(EXIT_FAILURE);
    }

    // Defines the archive's size (extends it to avoid Seg Fault)
    if (ftruncate(fd, SHARED_SIZE) == -1) {
        perror("ftruncate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char *shared_mem = mmap(NULL, SHARED_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap prog1");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    close(fd);

    printf("PROG 1 | Writing in shared memory...\n");
    const char *msg = "[PROG 1] Message from Prog1. Can you read me?";
    strncpy(shared_mem, msg, SHARED_SIZE);
    printf("PROG 1 | Message successfully written!\n");

    munmap(shared_mem, SHARED_SIZE);
    
    return 0;
}
