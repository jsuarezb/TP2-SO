
#include "include/pmem.h"
#include "include/paging.h"
#include <naiveConsole.h>

void * available_virtual_address = 0x80000000;

/**
 * Allocates a page and links it with a physical page
 */
void *
kalloc(void)
{
    available_virtual_address += PAGE_SIZE; // Go to the next page

    uint64_t virtual_address = (uint64_t) available_virtual_address;
    uint64_t pml4_index = (virtual_address >> 39) & 0x1FF;
    uint64_t pdp_index = (virtual_address >> 30) & 0x1FF;
    uint64_t pd_index = (virtual_address >> 21) & 0x1FF;
    uint64_t pt_index = (virtual_address >> 12) & 0x1FF;

    uint64_t block_address;

    // Check if the pdp pointed by pml4e is present
    PML4E pml4e = get_pml4e(pml4_index);
    if ((pml4e & PAGE_PRESENT) == 0) {
        block_address = (uint64_t) pmem_alloc();
        pml4e = create_pml4e(block_address >> 12, PAGE_US | PAGE_RW | PAGE_PRESENT);
        set_pml4e(pml4e, pml4_index);
    }

    // Check if the pd pointed by the pdpe is present
    PDPE pdpe = get_pdpe(pml4_index, pdp_index);
    if ((pdpe & PAGE_PRESENT) == 0) {
        block_address = (uint64_t) pmem_alloc();
        pdpe = create_pdpe(block_address >> 12, PAGE_US | PAGE_RW | PAGE_PRESENT);
        set_pdpe(pdpe, pml4_index, pdp_index);
    }

    // Check if the pt pointed by the pde is present
    PDE pde = get_pde(pml4_index, pdp_index, pd_index);
    if ((pde & PAGE_PRESENT) == 0) {
        block_address = (uint64_t) pmem_alloc();
        pde = create_pde(block_address >> 12, PAGE_US | PAGE_RW | PAGE_PRESENT);
        set_pde(pde, pml4_index, pdp_index, pd_index);
    }

    // Point to a physical memory address, set it to present and
    // return the virtual address used to set every level tables
    block_address = (uint64_t) pmem_alloc();

    PTE pte = create_pte(block_address >> 12, PAGE_US | PAGE_RW | PAGE_PRESENT);
    set_pte(pte, pml4_index, pdp_index, pd_index, pt_index);

    return available_virtual_address;
}

void
kfree(void * virtual_address)
{
    uint64_t address = (uint64_t) virtual_address;

    int pml4_index = (address >> 39) & 0x1FF;
    int pdp_index = (address >> 30) & 0x1FF;
    int pd_index = (address >> 21) & 0x1FF;
    int pt_index = (address >> 12) & 0x1FF;

    PTE pte = get_pte(pml4_index, pdp_index, pd_index, pt_index);

    ncPrint("PTE: ");
    ncPrintHex(pte);
    ncNewline();

    void * pmem_address = (void *) (pte & (PAGE_BASE_ADDR_MASK << PAGE_BASE_ADDR_OFF));

    set_pte(create_pte(0, PAGE_US | PAGE_RW), pml4_index, pdp_index, pd_index, pt_index);
    pmem_free(pmem_address);
}
