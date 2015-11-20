
#ifndef _KALLOC_H
#define _KALLOC_H

void * kalloc(void);

/**
 * Frees a page and frees the physical ram linked with it
 *
 * @param address   pointer to the address to free
 */
void kfree(void * address);

#endif
