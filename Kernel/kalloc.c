
#include "include/pmem.h"
#include "include/paging.h"
#include <naiveConsole.h>

// We start to give memory at the second half of the virtual address map
static void * available_virtual_address = 0xFFFF800000000000;

/**
 * Allocates a page and links it with a physical page
 */
void *
kalloc(void)
{
    int is_present;
    void * address;

    uint64_t virtual_address = (uint64_t) available_virtual_address;
    uint64_t pml4_index = (virtual_address >> 39) & 0x1FF;
    uint64_t pdp_index = (virtual_address >> 30) & 0x1FF;
    uint64_t pd_index = (virtual_address >> 21) & 0x1FF;
    uint64_t pt_index = (virtual_address >> 12) & 0x1FF;

    PML4E pml4e = get_pml4e(pml4_index);
    is_present = pml4e & PAGE_PRESENT;
    if (!is_present) {
        address = pmem_alloc();
        pml4e = create_pml4e((uint64_t) address >> PAGE_BASE_ADDR_OFF, PAGE_RW | PAGE_US | PAGE_PRESENT);
        set_pml4e(pml4e, pml4_index);

        clear_page_entries(address);
    }

    PDPE pdpe = get_pdpe(pml4_index, pdp_index);
    is_present = pdpe & PAGE_PRESENT;
    if (!is_present) {
        address = pmem_alloc();
        pdpe = create_pdpe((uint64_t) address >> PAGE_BASE_ADDR_OFF, PAGE_RW | PAGE_US | PAGE_PRESENT);
        set_pdpe(pdpe, pml4_index, pdp_index);

        clear_page_entries(address);
    }

    PDE pde = get_pde(pml4_index, pdp_index, pd_index);
    is_present = pde & PAGE_PRESENT;
    if (!is_present) {
        address = pmem_alloc();
        pde = create_pde((uint64_t) address >> PAGE_BASE_ADDR_OFF, PAGE_RW | PAGE_US | PAGE_PRESENT);
        set_pde(pde, pml4_index, pdp_index, pd_index);

        clear_page_entries(address);
    }

    uint64_t * block_address;

    // Point to a physical memory address, set it to present and
    // return the virtual address used to set every level tables
    block_address = pmem_alloc();

    PTE pte = create_pte((uint64_t) block_address >> 12, PAGE_US | PAGE_RW | PAGE_PRESENT);
    set_pte(pte, pml4_index, pdp_index, pd_index, pt_index);

    available_virtual_address += PAGE_SIZE; // Go to the next page

    return (void *) virtual_address;
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

    void * pmem_address = (void *) (pte & (PAGE_BASE_ADDR_MASK << PAGE_BASE_ADDR_OFF));

    set_pte(0, pml4_index, pdp_index, pd_index, pt_index);
    pmem_free(pmem_address);

    ncPrintHex(get_pte(pml4_index, pdp_index, pd_index, pt_index));
}
