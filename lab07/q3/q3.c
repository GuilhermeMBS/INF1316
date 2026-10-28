#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <assert.h>
#include <sys/time.h>


#define SHOW_ARCH 0


typedef enum
{
    ENV_SET = 7,
    PRINT_OK,
    MAP_OK,
    TIME_OK,
    WRITE_OK,
    DISK_OK
} ret_t;


struct timeval start, end;
struct stat sb;
off_t len;
char *p, *file_name;
int fd;


ret_t
init_environment(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    if (!S_ISREG(sb.st_mode)) {
        fprintf(stderr, "%s is not a file\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    file_name = argv[1];

    return ENV_SET;
}


ret_t
print_mapping_stats(void)
{
    long arch_size = sb.st_size;
    long page_size = (long)getpagesize();

    /**
     * The number of pages used is the lowest multiple of the page size
     * greater or equal to the archive size.
     * That's why we added 1 when the archive size is not a multiple of
     * the page size.
     */
    long n_pages = (arch_size / page_size) +
                   ((arch_size % page_size == 0) ? 0 : 1);

    printf("Archive Size: %ld\n", arch_size);
    printf("Page Size: %ld\n", page_size);
    printf("Number of Pages Used: %ld\n\n", n_pages);

    return PRINT_OK;
}


ret_t
calculate_elapsed_seconds(double *out_time)
{
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;

    if (microseconds < 0) {
        seconds -= 1;
        microseconds += 1000000;
    }

    double elapsed_time = seconds + (microseconds / 1000000.0);
    printf("Elapsed time: %.6f seconds\n", elapsed_time);
    *out_time = elapsed_time;

    return TIME_OK;
}


ret_t
map_archive(void)
{
    /**
     * MAP_POPULATE prefaults the page tables so that the benchmark loop
     * measures actual memory access rather than cold disk spin-up.
     */
    p = mmap(0, sb.st_size, PROT_READ, MAP_SHARED | MAP_POPULATE, fd, 0);

    if (p == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    gettimeofday(&start, NULL);
    for (len = 0; len < sb.st_size; len++) {
#if SHOW_ARCH
        putchar(p[len]);
#else
        volatile char dummy = p[len];
        (void)dummy;
#endif
    }
    gettimeofday(&end, NULL);

    if (munmap(p, sb.st_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    return MAP_OK;
}


ret_t
write_using_disk(void)
{
    fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("open disk");
        exit(EXIT_FAILURE);
    }

    char ch;
    ssize_t bytes_read;

    gettimeofday(&start, NULL);
    while ((bytes_read = read(fd, &ch, 1)) > 0) {
#if SHOW_ARCH
        putchar(ch);
#else
        volatile char dummy = ch;
        (void)dummy;
#endif
    }
    gettimeofday(&end, NULL);

    if (bytes_read == -1) {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return DISK_OK;
}


int
main(int argc, char *argv[])
{
    double mmap_time = 0.0;
    double disk_time = 0.0;

    assert(init_environment(argc, argv) == ENV_SET);
    printf("\n=============== FILE %s ===============\n", file_name);

    assert(map_archive() == MAP_OK);
    assert(print_mapping_stats() == PRINT_OK);

    puts("Mapping Elapsed Time:");
    assert(calculate_elapsed_seconds(&mmap_time) == TIME_OK);

    puts("\nUsing I/O in Disk for the same file:");
    assert(write_using_disk() == DISK_OK);
    assert(calculate_elapsed_seconds(&disk_time) == TIME_OK);

    printf("\nSpeedup: Mapped file was %.2fx faster than standard Disk I/O.\n",
            disk_time / mmap_time);

    return 0;
}
