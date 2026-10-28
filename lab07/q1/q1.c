#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#define STEPS 3                         // Number of steps including initial and final
#define INC 8                           // Increment for second step
#define m(i) ((char*)memorypool[i])     // Used to do calculus with assert


void *memorypool[STEPS];


void*
myallocate(int n)
{
    return sbrk(n);
}


void
init_memory_pool(void)
{
    memorypool[0] = sbrk(0);
}


void
reset_memory_pool(void)
{
    /**
     * The best way to reset would be to brk(memorypool[0]), but the statement
     * explicitly asks to use the myallocate function to deallocate.
     */
    myallocate(-INC);
    memorypool[STEPS - 1] = sbrk(0);
}


static inline int
_print_memory(void)
{
    for(int i = 0; i < STEPS; i++) {
        printf("Step %d | Heap Pointer: %p\n", i, memorypool[i]);
    }

    return 1;
}


int
main(void)
{
    // Step 01: Initial Heap Pointer
    init_memory_pool();

    // Step 02: Increment of INC Bytes
    myallocate(INC);
    memorypool[1] = myallocate(0);      // Save the value in second element of array
    assert(m(1) - m(0) == INC);         // New - Initial = Increment

    // Step 03: Reset Heap Pointer
    reset_memory_pool();
    assert(m(0) == m(2));               // Initial = Final
    
    // Print the Pointers From Each Step
    assert(_print_memory() == 1);

    return 0;
}
