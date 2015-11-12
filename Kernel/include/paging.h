#ifndef _PAGING_H
#define _PAGING_H

// A long mode virtual address is mapped as follows:
// Most significant 9 bits -> PML4 index
// Next 9 bits -> PDP index
// Next 9 bits -> PD index
// Next 9 bits -> PT index
// Last 12 bits -> index inside the page

typedef uint64_t bits_cr3;
typedef uint64_t bits_pml4e;
typedef uint64_t bits_pdpe;
typedef uint64_t bits_pde;
typedef uint64_t bits_pte;

typedef struct CR3 {
    uint64_t   pml4_address; // this address is aligned to 4Kb, giving addresses of 52 bits (last 12 set to 0)
    uint8_t    pcd; // caching policy
    uint8_t    pwt; // writethrough policy
} CR3;

typedef struct PML4E {
    uint8_t     nx;
    uint64_t    pdp_address;
    uint8_t     accessed;
    uint8_t     pcd; // chache flag
    uint8_t     pwt; // writethrough flag
    uint8_t     user_supervisor;  // 0 to kernel, 1 to user
    uint8_t     read_write; // 0 read-only, 1 read-write
    uint8_t     present;
} PML4E;

typedef struct PDPE {
    uint8_t     is_bottom_level;
    uint8_t     nx;
    uint64_t    pd_address;
    uint8_t     accessed;
    uint8_t     pcd; // chache flag
    uint8_t     pwt; // writethrough flag
    uint8_t     user_supervisor;  // 0 to kernel, 1 to user
    uint8_t     read_write; // 0 read-only, 1 read-write
    uint8_t     present;
    uint8_t     pat;
    uint8_t     global;
    uint8_t     dirty;
} PDPE;

void init_paging(void);

/**
 * Get the cr3 value from the processor and stores it in `cr3` struct
 *
 * @param cr3   pointer to a cr3_register struct
 */
void get_cr3(CR3 * cr3);

/**
 * Set the cr3 register value with the given values in `cr3` struct
 *
 * @param cr3   pointer to a cr3_register struct
 */
void set_cr3(CR3 * cr3);

void get_pml4e(PML4E * entry, int pml4e_index);

void set_pml4e(PML4E * entry, int pml4e_index);

void get_pdpe(PDPE * entry, int pml4e_index, int pdp_index);

void set_pdpe(PDPE * entry, int pml4e_index, int pdp_index);

#endif
