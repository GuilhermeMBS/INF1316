#define _GNU_SOURCE // Needed to use the flag MREMAP_MAYMOVE in mremap()


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

    /**
     * Separate the actual file size (write offset) from the initial mapping size.
     * If the file is empty (0 bytes), we map 1 dummy byte to satisfy mmap(),
     * but keep write_offset at 0 so we don't leave a leading NULL byte at index 0.
     */
    size_t write_offset = (size_t)sb.st_size;
    size_t old_map_size = (write_offset > 0) ? write_offset : 1;

    char *p = mmap(NULL, old_map_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    const char *new_text = "==> Text appended using mremap.\n";
    size_t text_len = strlen(new_text);
    size_t new_size = write_offset + text_len;

    if (ftruncate(fd, (off_t)new_size) == -1) {
        perror("ftruncate");
        munmap(p, old_map_size);
        close(fd);
        exit(EXIT_FAILURE);
    }

    char *new_p = mremap(p, old_map_size, new_size, MREMAP_MAYMOVE);
    if (new_p == MAP_FAILED) {
        perror("mremap");
        munmap(p, old_map_size);
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Write starting at the true end of the original file (0 if initially empty)
    memcpy(&new_p[write_offset], new_text, text_len);

    printf("Mapping resized from %zu to %zu bytes at address %p\n",
           write_offset, new_size, (void *)new_p);

    if (munmap(new_p, new_size) == -1) {
        perror("munmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
