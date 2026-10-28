#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>


#define FILE_NAME "data.txt"


int
main(void)
{
    int fd = open(FILE_NAME, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t prev_size = sb.st_size;
    const char *new_text = "==> Extension Text\n";
    size_t new_size = prev_size + strlen(new_text);

    // Extends the archive's size, preventing Seg Fault
    if (ftruncate(fd, new_size) == -1) {
        perror("ftruncate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char *p = mmap(NULL, new_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    close(fd);

    memcpy(&p[prev_size], new_text, strlen(new_text));
    puts("\nNew text successfully added!");
    printf("Previous Size: %zu bytes\nNew Size: %zu bytes.\n", 
           prev_size, new_size);

    munmap(p, new_size);

    return 0;
}
