#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_KEY 999
#define MIN_KEY 0
#define NUM_OPS 10000

typedef enum {
    OP_READ,
    OP_WRITE
} op_type_t;

void
generate_operations(int n)
{
    FILE *file = fopen("log.txt", "w");
    if (!file) {
        perror("Error creating log.txt");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int read_count = 0, write_count = 0;
    
    // Tracks the last written key to simulate Temporal Locality
    int last_written_key = -1;

    for (int i = 0; i < n; i++) {
        op_type_t op;
        int key;

        // 30% chance to immediately read the last written data (Cache Theory)
        if (last_written_key != -1 && (rand() % 100) < 30) {
            op = OP_READ;
            key = last_written_key;
        } else {
            op = rand() % 2;
            key = (rand() % (MAX_KEY - MIN_KEY + 1)) + MIN_KEY;
        }

        if (op == OP_READ) {
            fprintf(file, "R %d\n", key);
            read_count++;
        } else {
            int value = (rand() % 4991) + 10;
            fprintf(file, "W %d %d\n", value, key);
            write_count++;
            last_written_key = key;
        }
    }
    
    fclose(file);
    printf("[SUCCESS] log.txt generated with %d ops (Reads: %d, Writes: %d)\n", n, read_count, write_count);
    printf("[INFO] Temporal Locality enabled (30%% read-after-write bias).\n");
}

int
main(void)
{
    generate_operations(NUM_OPS);
    return EXIT_SUCCESS;
}
