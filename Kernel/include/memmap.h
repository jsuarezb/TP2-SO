
#ifndef MEMMAP_H
#define MEMMAP_H

#define BITS_IN_BYTE    8
#define BLOCK_SIZE      ((uint64_t) 4 * 1024) // 4kb

#define FREE_BLOCK  0
#define ALLC_BLOCK  1

#include <stdint.h>

typedef struct {
    char * address;
    void * offset;
    uint64_t size;
    uint64_t last_alloc;
} MemoryBitmap;

void init_bitmap(MemoryBitmap * bitmap, char * address, void * offset, uint64_t memory_size);

void * alloc_block(MemoryBitmap * bitmap);

void free_block(MemoryBitmap * bitmap, void * address);

#endif
