
#ifndef _KALLOC_H
#define _KALLOC_H

#define HEAP_VIRTUAL_START  ((uint64_t) 0xFFFF800000000000)

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

#endif
