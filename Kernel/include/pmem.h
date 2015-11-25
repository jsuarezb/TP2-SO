#ifndef _PMEM_H
#define _PMEM_H

#include <stdint.h>

// 1 GB de physical memory offset
#define PMEM_OFFSET ((uint64_t) 1 * 1024 * 1024 * 1024)

#define PMEM_SIZE    ((uint64_t) 4 * 1024 * 1024 * 1024) // 4 GB

/**
 * Initialize physical memory manager
 */
void init_pmem();

/**
 * Alloc a block of physical memory
 */
void * pmem_alloc();

/**
 * Free a block of physical memory
 */
void pmem_free(void *);

#endif
