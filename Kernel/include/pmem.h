#ifndef _PMEM_H
#define _PMEM_H

#include <stdint.h>

#define PMEM_OFFSET ((uint64_t) 0x40000000) // 512 MB de offset

#define MEM_SIZE    ((uint64_t) 4 * 1024 * 1024 * 1024) // 4 GB
#define BLOCK_SIZE  (4 * 1024) // 4 KB
#define BLOCKS      (MEM_SIZE / BLOCK_SIZE) // 4 MB

#define FREE_BLOCK  0
#define ALLC_BLOCK  1

#define BITS_IN_BYTE 8

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
