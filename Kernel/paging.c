
#include "include/lib.h"
#include "include/paging.h"
#include "include/naiveConsole.h"

static PML4E * get_pml4_table();

static PDPE * get_pdp_table(int pml4_index);

static PDE * get_pd_table(int pml4_index, int pdp_index);

static PTE * get_pt_table(int pml4_index, int pdp_index, int pd_index);

static PML4E * pml4_table  = 0x800000;
static PDPE * pdpe_table1   = 0x801000;

void
init_paging(void)
{
    CR3 cr3 = create_cr3(0x800, 0);
    pml4_table[0]   = create_pml4e(0x801, PAGE_US | PAGE_RW | PAGE_PRESENT);
    pdpe_table1[0]  = create_pdpe(0x000, PAGE_US | PAGE_RW | PAGE_PRESENT | PAGE_BOTTOM);
    _asm_set_cr3(cr3);

    PDPE pdpe = create_pdpe(0x802, PAGE_US | PAGE_RW | PAGE_PRESENT);
    set_pdpe(pdpe, 0, 1);

    PDE pde = create_pde(0x803, PAGE_US | PAGE_RW | PAGE_PRESENT);
    set_pde(pde, 0, 1, 0);

    PTE pte = create_pte(0x40000, PAGE_US | PAGE_RW | PAGE_PRESENT);
    set_pte(pte, 0, 1, 0, 0);

    // PDE pde = create_pde(0x950, PAGE_US | PAGE_RW | PAGE_PRESENT);
    // set_pde(pde, 0, 1, 0);
    //
    // PTE pte = create_pte(0x40000, PAGE_US | PAGE_RW | PAGE_PRESENT);
    // set_pte(pte, 0, 1, 0, 0);

    char * i = (char *) 0x40000000 + 4 * 1024 - 1;
    i[0] = 'a';
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
    return flags | ((address << CR3_PML4_ADDR_OFF) & CR3_PML4_ADDR_MASK);
}

PML4E
create_pml4e(uint64_t address, uint64_t flags)
{
    return flags | ((address << PAGE_BASE_ADDR_OFF) & PAGE_BASE_ADDR_MASK);
}

PDPE
create_pdpe(uint64_t address, uint64_t flags)
{
    return (flags & PAGE_BOTTOM
        ? ((address << PAGE_1GB_BASE_ADDR_OFF) & PAGE_1GB_BASE_ADDR_MASK) | flags
        : ((address << PAGE_BASE_ADDR_OFF) & PAGE_BASE_ADDR_MASK) | flags);
}

PDE
create_pde(uint64_t address, uint64_t flags)
{
    return flags | ((address << PAGE_BASE_ADDR_OFF) & PAGE_BASE_ADDR_MASK);
}

PTE
create_pte(uint64_t address, uint64_t flags)
{
    return flags | ((address << PAGE_BASE_ADDR_OFF) & PAGE_BASE_ADDR_MASK);
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
