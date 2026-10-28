#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>


#define FILE_NAME "shared_data.bin"
#define SHARED_SIZE 4096


int
main(void)
{
    // Open file with read only
    int fd = open(FILE_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open prog2 (did you run prog1 first?)");
        exit(EXIT_FAILURE);
    }

    char *shared_mem = mmap(NULL, SHARED_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (shared_mem == MAP_FAILED) {
        perror("mmap prog2");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    close(fd);

    printf("PROG 2 | Reading Shared Memory:\n");
    printf("========================================\n");
    printf("%s\n", shared_mem);
    printf("========================================\n");

    munmap(shared_mem, SHARED_SIZE);
    
    return 0;
}
