
#ifndef _KALLOC_H
#define _KALLOC_H

#define HEAP_VIRTUAL_START  ((void *) 0xFFFF800000000000)
#define VMEM_SIZE    ((uint64_t) 4 * 1024 * 1024 * 1024) // 4 GB

void init_kalloc(void);

/**
 * Allocates a page virtually and returns the virtual address allocated
 *
 * @return a pointer to the 4kb page
 */
void * kalloc(void);

/**
 * Frees a page and frees the physical ram linked with it
 *
 * @param address   pointer to the address to free
 */
void kfree(void * address);

/**
 * Allocates a block into the linear address specified.
 * This will allocate all tables needed to reach Page Table level.
 *
 * @param virtual_address   linear address to map
 */
void * virtual_kalloc(void * address);

#endif
