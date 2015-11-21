#ifndef _PAGING_H
#define _PAGING_H

#include <stdint.h>

#define PAGE_SIZE   (4 * 1024)

#define CR3_PCD     (1 << 4)
#define CR3_PWT     (1 << 3)
#define CR3_PML4_ADDR_OFF   12
#define CR3_PML4_ADDR_MASK  (uint64_t) 0xFFFFFFFFFF

#define PAGE_NX     ((uint64_t) 1 << 63)
#define PAGE_GLOBAL ((uint64_t) 1 << 8)
#define PAGE_BOTTOM ((uint64_t) 1 << 7)
#define PAGE_DIRTY  ((uint64_t) 1 << 6)
#define PAGE_PCD    ((uint64_t) 1 << 4)
#define PAGE_PWT    ((uint64_t) 1 << 3)
#define PAGE_US     ((uint64_t) 1 << 2)
#define PAGE_RW     ((uint64_t) 1 << 1)
#define PAGE_PRESENT    ((uint64_t) 1)

#define PAGE_PDP_PAT    ((uint64_t) 1 << 12)
#define PAGE_PTE_PAT    ((uint64_t) 1 << 7)

#define PAGE_BASE_ADDR_MASK     ((uint64_t) 0xFFFFFFFFFF)
#define PAGE_BASE_ADDR_OFF      12

#define PAGE_1GB_BASE_ADDR_MASK ((uint64_t) 0x3FFFFF)
#define PAGE_1GB_BASE_ADDR_OFF  30

typedef uint64_t CR3;
typedef uint64_t PML4E;
typedef uint64_t PDPE;
typedef uint64_t PDE;
typedef uint64_t PTE;

/**
 * Creates a new paging tables structure and identity-maps the first
 * GB of kernel memory.
 */
void init_paging(void);

/**
 * Returns a PML4 entry
 *
 * @param pml4_index   index of the entry inside PML4 table
 * @return a PML4 entry
 */
PML4E get_pml4e(int pml4_index);

/**
 * Sets a PML4 entry
 *
 * @param pml4e         entry to set
 * @param pml4_index    index of the entry inside PML4 table
 */
void set_pml4e(PML4E pml4, int pml4_index);

/**
 * Returns a PDP entry
 *
 * @param pml4_index    index of the entry inside PML4 table
 * @param pdp_index     index of the entry inside PDP table
 * @return a PDP entry
 */
PDPE get_pdpe(int pml4_index, int pdp_index);

/**
 * Sets a PDP entry
 *
 * @param pdpe           entry to set
 * @param pml4_index    index of the entry inside PML4 table
 * @param pdp_index     index of the entry inside PDP table
 */
void set_pdpe(PDPE pdpe, int pml4_index, int pdp_index);

/**
 * Returns a PD entry
 *
 * @param pml4_index    index of the entry inside PML4 table
 * @param pdp_index     index of the entry inside PDP table
 * @param pd_index     index of the entry inside PD table
 * @return a PD entry
 */
PDE get_pde(int pml4_index, int pdp_index, int pd_index);

/**
 * Sets a PD entry
 *
 * @param pde           entry to set
 * @param pml4_index    index of the entry inside PML4 table
 * @param pdp_index     index of the entry inside PDP table
 * @param pd_index      index of the entry inside PD table
 */
void set_pde(PDE pde, int pml4_index, int pdp_index, int pd_index);

/**
 * Returns a PT entry
 *
 * @param pml4_index    index of the entry inside PML4 table
 * @param pdp_index     index of the entry inside PDP table
 * @param pd_index      index of the entry inside PD table
 * @param pt_index      index of the entry inside PT table
 * @return a PT entry
 */
PTE get_pte(int pml4_index, int pdp_index, int pd_index, int pt_index);

/**
 * Sets a PT entry
 *
 * @param pte           entry to set
 * @param pml4_index    index of the entry inside PML4 table
 * @param pdp_index     index of the entry inside PDP table
 * @param pd_index      index of the entry inside PD table
 * @param pt_index      index of the entry inside PT table
 */
void set_pte(PTE pte, int pml4_index, int pdp_index, int pd_index, int pt_index);

/**
 * Creates a CR3 register entry
 *
 * @param address   pml4 address
 * @param flags     cr3 flags
 */
CR3 create_cr3(uint64_t address, uint64_t flags);

/**
 * Creates a PML4 entry
 *
 * @param address   pdp address
 * @param flags     pml4 entry flags
 */
PML4E create_pml4e(uint64_t address, uint64_t flags);

/**
 * Creates a PDP entry
 *
 * @param address   pd address
 * @param flags     pdp entry flags
 */
PDPE create_pdpe(uint64_t address, uint64_t flags);

/**
 * Creates a PD entry
 *
 * @param address   pt address
 * @param flags     pd entry flags
 */
PDE create_pde(uint64_t address, uint64_t flags);

/**
 * Creates a PT entry
 *
 * @param address   physical frame address
 * @param flags     pt entry flags
 */
PTE create_pte(uint64_t address, uint64_t flags);


/**
 * Clears a page, setting every entry to 0
 *
 * @param base_address  Base address of the page
 */
void clear_page_entries(uint64_t * base_address);

#endif
