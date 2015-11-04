#ifndef _PMEM_H
#define _PMEM_H

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
