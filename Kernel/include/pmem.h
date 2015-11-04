#ifndef _PMEM_H
#define _PMEM_H

/**
 * Sets an offset to the memory available to map.
 * This will become handy to avoid overlapping kernel specific memory.
 */
void set_pmem_offset(void *);

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
