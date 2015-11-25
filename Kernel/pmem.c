
/**
 * Physical memory manager
 */

#include <stdint.h>
#include "include/pmem.h"
#include "include/memmap.h"

static MemoryBitmap bitmap;

void
init_pmem()
{
    init_bitmap(&bitmap, (void *) 0x10000000, (void *) PMEM_OFFSET, PMEM_SIZE);
}

void
pmem_free(void * address)
{
    free_block(&bitmap, address);
}

void *
pmem_alloc()
{
    return alloc_block(&bitmap);
}
