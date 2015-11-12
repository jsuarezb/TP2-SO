
#include "include/lib.h"
#include "include/paging.h"
#include "include/naiveConsole.h"

/**
 * Writes the cr3 register in `bits` given the `cr3` struct
 *
 * @param bits  return parameter, pointer to 64 bits value
 * @param cr3   pointer to the cr3 struct
 */
static void write_cr3_bits(bits_cr3 * bits, CR3 * cr3);

/**
 * Write the specified values inside the specified cr3 struct
 *
 * @param cr3   return parameter, pointer to the cr3 struct to write
 * @param pml4_address  pml4_address to write into cr3 struct
 * @param pcd   pcd value to write into cr3 struct
 * @param pwt   pwt value to write into cr3 struct
 */
static void write_cr3_struct(CR3 * cr3, uint64_t pml4_address,
    uint8_t pcd, uint8_t pwt);

/**
 * Populates `cr3` struct with values given in `bits`
 *
 * @param cr3   return parameter, pointer to the struct to populate
 * @param bits  cr3 register value
 */
static void populate_cr3_struct(CR3 * cr3, bits_cr3 bits);

/**
 * Returns the address of the PML4 table set into CR3
 *
 * @return  pointer to PML4 table
 */
static bits_pml4e * get_pml4_table();

/**
 * Writes `entry` values into `bits` pointer
 *
 * @param bits  return parameter, pointer to the pml4e
 * @param entry pointer to the PML4E struct
 */
static void write_pml4_bits(bits_pml4e * bits, PML4E * entry);

/**
 * Populates `entry` struct with values. Those values that can't be set by
 * parameters will be assigned to 0.
 *
 * @param entry             return parameter, pointer to the struct that will
 *                          be populated
 * @param nx                no execute bit
 * @param pdp_address       pdp page address
 * @param accessed          accessed bit
 * @param pcd               pcd bit
 * @param pwt               pwt bit
 * @param user_supervisor   u/s bit, set to 0 to allow access only to supervisor
 *                          set to 1 to access with user and supervisor
 * @param read_write        r/w bit, set to 1 to allow read and write, set to 0
 *                          to restrict access to read-only
 * @param present           present bit
 */
static void write_pml4_struct(PML4E * entry, uint8_t nx, uint64_t pdp_address,
    uint8_t accessed, uint8_t pcd, uint8_t pwt, uint8_t user_supervisor,
    uint8_t read_write, uint8_t present);

/**
 * Populates `entry` with the values encoded in `bits`
 *
 * @param entry return parameter, pointer to the PML4E struct
 * @param bits  bits that represent the PML4E
 */
static void populate_pml4_struct(PML4E * entry, bits_pml4e bits);

/**
 * Returns the pdp base address pointed by `pml4_index`
 *
 * @param pml4_index    index of the entry inside PML4 table
 */
static bits_pdpe * get_pdp_table(int pml4_index);

static void write_pdp_bits(bits_pdpe * bits, PDPE * entry);

/**
 * Populates `entry` struct with values.
 *
 * @param entry             {@link write_pml4_struct}
 * @param nx                {@link write_pml4_struct}
 * @param pdp_address       {@link write_pml4_struct}
 * @param pat               page attribute bit, present if the page is the
 *                          lowest level
 * @param accessed          {@link write_pml4_struct}
 * @param pcd               {@link write_pml4_struct}
 * @param pwt               {@link write_pml4_struct}
 * @param user_supervisor   {@link write_pml4_struct}
 * @param read_write        {@link write_pml4_struct}
 * @param present           {@link write_pml4_struct}
 */
static void write_pdp_struct(PDPE * entry, uint8_t nx, uint64_t pd_address,
    uint8_t pat, uint8_t global, uint8_t is_bottom_level, uint8_t dirty,
    uint8_t accessed, uint8_t pcd, uint8_t pwt, uint8_t user_supervisor,
    uint8_t read_write, uint8_t present);

static void populate_pdp_struct(PDPE * entry, bits_pdpe bits);


// Code

static bits_pml4e * pml4_table  = 0x800000;
static bits_pdpe * pdpe_table   = 0x801000;

void
init_paging(void)
{
    PML4E pml4e;
    write_pml4_struct(&pml4e, 0, 0x801, 0, 0, 1, 1, 1, 1);
    write_pml4_bits(pml4_table, &pml4e);

    PDPE pdpe;
    write_pdp_struct(&pdpe, 0, 0x00, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1);
    write_pdp_bits(pdpe_table, &pdpe);

    CR3 cr3_struct;
    write_cr3_struct(&cr3_struct, 0x800, 0, 1);
    set_cr3(&cr3_struct);
}

void
get_cr3(CR3 * cr3)
{
    bits_cr3 bits;

    bits = _asm_get_cr3();
    populate_cr3_struct(cr3, bits);
}

void
set_cr3(CR3 * cr3)
{
    bits_cr3 bits;

    write_cr3_bits(&bits, cr3);
    _asm_set_cr3(bits);
}

void
get_pml4e(PML4E * entry, int pml4e_index)
{
    bits_pml4e * pml4 = get_pml4_table();
    populate_pml4_struct(entry, pml4[pml4e_index]);
}

void
set_pml4e(PML4E * entry, int pml4e_index)
{
    bits_pml4e * pml4 = get_pml4_table(); // align to 4 KB
    write_pml4_bits(&pml4[pml4e_index], entry);
}

void
get_pdpe(PDPE * entry, int pml4e_index, int pdpe_index)
{
    bits_pdpe * pdp = get_pdp_table(pml4e_index);
    populate_pdp_struct(entry, pdp[pdpe_index]);
}

void
set_pdpe(PDPE * entry, int pml4e_index, int pdpe_index)
{
    bits_pdpe * pdp = get_pdp_table(pml4e_index);
    write_pdp_bits(&pdp[pdpe_index], entry);
}

static void
write_cr3_bits(bits_cr3 * bits, CR3 * cr3)
{
    *bits = 0x00;

    *bits = cr3->pml4_address << 12
        | cr3->pcd << 4
        | cr3->pwt << 3;
}

static void
write_cr3_struct(CR3 * cr3, uint64_t pml4_address, uint8_t pcd,
    uint8_t pwt)
{
    cr3->pml4_address = pml4_address & 0xFFFFFFFFFF;
    cr3->pcd = pcd & 0x01;
    cr3->pwt = pwt & 0x01;
}

static void
populate_cr3_struct(CR3 * cr3, bits_cr3 bits)
{
    write_cr3_struct(
        cr3,
        bits >> 12,
        bits >> 4,
        bits >> 3
    );
}

static bits_pml4e *
get_pml4_table()
{
    CR3 cr3;
    get_cr3(&cr3);

    return (void *) (cr3.pml4_address << 12);
}

static void
write_pml4_bits(bits_pml4e * bits, PML4E * entry)
{
    *bits = 0;

    *bits = entry->nx & 0x01;
    *bits <<= 63; // avoid nx overflow

    *bits = entry->pdp_address << 12
        | entry->accessed << 5
        | entry->pcd << 4
        | entry->pwt << 3
        | entry->user_supervisor << 2
        | entry->read_write << 1
        | entry->present;
}

static void
write_pml4_struct(PML4E * entry, uint8_t nx, uint64_t pdp_address,
    uint8_t accessed, uint8_t pcd, uint8_t pwt, uint8_t user_supervisor,
    uint8_t read_write, uint8_t present)
{
    entry->nx       = nx & 0x01;
    entry->pdp_address  = pdp_address & 0xFFFFFFFFFF;
    entry->accessed = accessed & 0x01;
    entry->pcd      = pcd & 0x01;
    entry->pwt      = pwt & 0x01;
    entry->user_supervisor  = user_supervisor & 0x01;
    entry->read_write       = read_write & 0x01;
    entry->present  = present & 0x01;
}

static void
populate_pml4_struct(PML4E * entry, bits_pml4e bits)
{
    write_pml4_struct(
        entry,
        bits >> 63,
        bits >> 12,
        bits >> 5,
        bits >> 4,
        bits >> 3,
        bits >> 5,
        bits >> 5,
        bits >> 0
    );
}

static bits_pdpe *
get_pdp_table(int pml4_index)
{
    PML4E pml4e;
    get_pml4e(&pml4e, pml4_index);

    return (void *) (pml4e.pdp_address << 12);
}

static void
write_pdp_bits(bits_pdpe * bits, PDPE * entry)
{
    *bits = 0;

    *bits = entry->nx & 0x01;
    *bits <<= 63; // avoid nx overflow

    *bits = entry->is_bottom_level << 7
        | entry->accessed << 5
        | entry->pcd << 4
        | entry->pwt << 3
        | entry->user_supervisor << 2
        | entry->read_write << 1
        | entry->present;

    if (entry->is_bottom_level == 0) {
        *bits |= entry->pd_address << 12;
    } else {
        *bits |= entry->pd_address << 30 & 0x3FFFFF;
        *bits |= entry->pat << 12;
        *bits |= entry->global << 8;
        *bits |= entry->dirty << 6;
    }
}

static void
write_pdp_struct(PDPE * entry, uint8_t nx, uint64_t pd_address, uint8_t pat,
    uint8_t global, uint8_t is_bottom_level, uint8_t dirty, uint8_t accessed,
    uint8_t pcd, uint8_t pwt, uint8_t user_supervisor, uint8_t read_write,
    uint8_t present)
{
    entry->nx           = nx & 0x01;
    entry->pat          = pat & 0x01;
    entry->global       = global & 0x01;
    entry->is_bottom_level  = is_bottom_level & 0x01;
    entry->dirty            = dirty & 0x01;
    entry->accessed         = accessed & 0x01;
    entry->pcd              = pcd & 0x01;
    entry->pwt              = pwt & 0x01;
    entry->user_supervisor  = user_supervisor & 0x01;
    entry->read_write       = read_write & 0x01;
    entry->present          = present & 0x01;
    entry->pd_address   = is_bottom_level
        ? pd_address >> 18 & 0x3FFFFF : pd_address & 0xFFFFFFFFFF;
}

static void
populate_pdp_struct(PDPE * entry, bits_pdpe bits)
{
    write_pdp_struct(
        entry,
        bits >> 63,
        bits >> 12,
        bits >> 12,
        bits >> 8,
        bits >> 7,
        bits >> 6,
        bits >> 5,
        bits >> 4,
        bits >> 3,
        bits >> 2,
        bits >> 1,
        bits >> 0
    );
}
