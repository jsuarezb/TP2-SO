#ifndef _PMEM_H
#define _PMEM_H

#include <stdint.h>

// 2 GB de physical memory offset
#define PMEM_OFFSET ((uint64_t) 1 * 1024 * 1024 * 1024)

#define MEM_SIZE    ((uint64_t) 4 * 1024 * 1024 * 1024) // 4 GB
#define BLOCK_SIZE  (4 * 1024) // 4 KB
#define BLOCKS      (MEM_SIZE / BLOCK_SIZE) // 4 MB

#define FREE_BLOCK  0
#define ALLC_BLOCK  1

#define BITS_IN_BYTE    8
#define BITMAP_SIZE     (BLOCKS / BITS_IN_BYTE)

/**
 * Alloc a block of physical memory
 */
void * pmem_alloc();

/**
 * Free a block of physical memory
 */
void pmem_free(void *);

/**
 * Initialize physical memory manager
 */
void init_pmem();

#endif
