
/**
 * Phyisical memory manager
 */

#include <stdint.h>
#include "include/pmem.h"

#define BLOCKS 1024
#define BLOCK_SIZE 4 * 1024
#define PMEM_OFFSET (void *) 0x10000

// Physical memory offset, blocks will be given after this address
static void * mem_offset = PMEM_OFFSET;
static void * block_stack[BLOCKS];

static int stack_index = 0;

void *
pmem_alloc()
{
    if (stack_index >= 0)
        return block_stack[stack_index--];

    // Null address
    return 0;
}

void
pmem_free(void * address)
{
    if (address < mem_offset || address >= mem_offset + (BLOCK_SIZE * BLOCKS))
        return;

    if (stack_index >= BLOCKS)
        return;

    address -= (uint64_t) address % BLOCK_SIZE; // align address to BLOCK_SIZE
    block_stack[stack_index++] = address; // push available address again
}

void
init_pmem()
{
    void * add;
    uint64_t i = 0;
    while (i < BLOCKS) {
        add = (void *) (i * BLOCK_SIZE + mem_offset);
        block_stack[i] = add;
        i++;
    }

    stack_index = BLOCKS - 1;
}

uint64_t
mem_available()
{
    return (stack_index + 1) * BLOCK_SIZE;
}
