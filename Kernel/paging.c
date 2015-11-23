
#include "include/lib.h"
#include "include/paging.h"
#include "include/naiveConsole.h"

#include <stdint.h>

static PML4E * get_pml4_table();

static PDPE * get_pdp_table(int pml4_index);

static PDE * get_pd_table(int pml4_index, int pdp_index);

static PTE * get_pt_table(int pml4_index, int pdp_index, int pd_index);

static PML4E * pml4_table  = (PML4E *) 0x800000;
static PDPE * pdpe_table1   = (PDPE *) 0x801000;

void
init_paging(void)
{
    CR3 cr3 = create_cr3((uint64_t) pml4_table >> 12, 0);
    clear_page_entries(pml4_table);

    pml4_table[0]   = create_pml4e((uint64_t) pdpe_table1 >> 12, PAGE_US | PAGE_RW | PAGE_PRESENT);
    clear_page_entries(pdpe_table1);

    // Identity map 4GB of memory
    pdpe_table1[0]  = create_pdpe(0x00, PAGE_US | PAGE_RW | PAGE_PRESENT | PAGE_BOTTOM);
    pdpe_table1[1]  = create_pdpe(0x01, PAGE_US | PAGE_RW | PAGE_PRESENT | PAGE_BOTTOM);
    pdpe_table1[2]  = create_pdpe(0x02, PAGE_US | PAGE_RW | PAGE_PRESENT | PAGE_BOTTOM);
    pdpe_table1[3]  = create_pdpe(0x03, PAGE_US | PAGE_RW | PAGE_PRESENT | PAGE_BOTTOM);

    _asm_set_cr3(cr3);
}

PML4E
get_pml4e(int pml4e_index)
{
    PML4E * table = get_pml4_table();
    return table[pml4e_index];
}

void
set_pml4e(PML4E entry, int pml4_index)
{
    PML4E * table = get_pml4_table();
    table[pml4_index] = entry;
}

PDPE
get_pdpe(int pml4_index, int pdp_index)
{
    PDPE * table = get_pdp_table(pml4_index);
    return table[pdp_index];
}

void
set_pdpe(PDE entry, int pml4_index, int pdp_index)
{
    PDE * table = get_pdp_table(pml4_index);

    table[pdp_index] = entry;
}

PDE
get_pde(int pml4_index, int pdp_index, int pd_index)
{
    PDE * table = get_pd_table(pml4_index, pdp_index);
    return table[pd_index];
}

void
set_pde(PDE entry, int pml4_index, int pdp_index, int pd_index)
{
    PDE * table = get_pd_table(pml4_index, pdp_index);
    table[pd_index] = entry;
}

PTE
get_pte(int pml4_index, int pdp_index, int pd_index, int pt_index)
{
    PTE * table = get_pt_table(pml4_index, pdp_index, pd_index);
    return table[pt_index];
}

void
set_pte(PTE entry, int pml4_index, int pdp_index, int pd_index, int pt_index)
{
    PTE * table = get_pt_table(pml4_index, pdp_index, pd_index);
    table[pt_index] = entry;
}

CR3
create_cr3(uint64_t address, uint64_t flags)
{
    return flags | ((address & CR3_PML4_ADDR_MASK) << CR3_PML4_ADDR_OFF);
}

PML4E
create_pml4e(uint64_t address, uint64_t flags)
{
    return flags | ((address & PAGE_BASE_ADDR_MASK) << PAGE_BASE_ADDR_OFF);
}

PDPE
create_pdpe(uint64_t address, uint64_t flags)
{
    return (flags & PAGE_BOTTOM
        ? ((address & PAGE_1GB_BASE_ADDR_MASK) << PAGE_1GB_BASE_ADDR_OFF) | flags
        : ((address & PAGE_BASE_ADDR_MASK) << PAGE_BASE_ADDR_OFF) | flags);
}

PDE
create_pde(uint64_t address, uint64_t flags)
{
    return flags | ((address & PAGE_BASE_ADDR_MASK) << PAGE_BASE_ADDR_OFF);
}

PTE
create_pte(uint64_t address, uint64_t flags)
{
    return flags | ((address & PAGE_BASE_ADDR_MASK) << PAGE_BASE_ADDR_OFF);
}

void
clear_page_entries(uint64_t * address)
{
    if (address == 0)
        return;

    int i;
    for (i = 0; i < 512; i++)
        address[i] = 0;
}

void *
get_physical_address(void * address)
{
    int pml4_index = ((uint64_t) address >> 39) & 0x1FF;
    int pdp_index = ((uint64_t) address >> 30) & 0x1FF;
    int pd_index = ((uint64_t) address >> 21) & 0x1FF;
    int pt_index = ((uint64_t) address >> 12) & 0x1FF;

    PTE e = get_pte(pml4_index, pdp_index, pd_index, pt_index);

    return (void *) (e & (PAGE_BASE_ADDR_MASK << PAGE_BASE_ADDR_OFF));
}

static PML4E *
get_pml4_table()
{
    CR3 cr3 = _asm_get_cr3();

    return (PML4E *) (cr3 & (CR3_PML4_ADDR_MASK << CR3_PML4_ADDR_OFF));
}

static PDPE *
get_pdp_table(int pml4_index)
{
    PML4E * table = get_pml4_table();

    return (PDPE *) (table[pml4_index] & (PAGE_BASE_ADDR_MASK << PAGE_BASE_ADDR_OFF));
}

static PDE *
get_pd_table(int pml4_index, int pdp_index)
{
    PDPE * table = get_pdp_table(pml4_index);

    return (PDE *) (table[pdp_index] & (PAGE_BASE_ADDR_MASK << PAGE_BASE_ADDR_OFF));
}

static PTE *
get_pt_table(int pml4_index, int pdp_index, int pd_index)
{
    PDE * table = get_pd_table(pml4_index, pdp_index);

    return (PTE *) (table[pd_index] & (PAGE_BASE_ADDR_MASK << PAGE_BASE_ADDR_OFF));
}
