
#include "include/paging.h"

// typedef struct {
//     char[12]    reserved_0;
//     char[40]    pml4_address; // this address is aligned to 4Kb, giving addresses of 52 bits (last 12 set to 0)
//     char[7]     reserved_1;
//     char        pcd; // caching policy
//     char        pwt; // writethrough policy
//     char[3]     reserved_2;
// } cr3_register;
//
// typedef struct {
//     char[16]    sign_extended;
//     char[9]     pml4_offset;
//     char[9]     pdp_offset;
//     char[9]     pd_offset;
//     char[9]     pt_offset;
//     char[12]    page_offset;
// } virt_address;
//
// typedef struct {
//     char        nx;
//     char[11]    avl_0;
//     char[40]    address;
//     char[3]     avl_1;
//     char[3]     zero_0;
//     char        accessed;
//     char        pcd; // chache flag
//     char        pwd; // chache flag
//     char        user_supervisor;  // 0 to kernel, 1 to user
//     char        read_write; // 0 read-only, 1 read-write
//     char        present;
// } pml4_entry;
//
// typedef struct {
//     char        nx;
//     char[11]    avl_0;
//     char[40]    address;
//     char[3]     avl_1;
//     char[3]     zero_0;
//     char        accessed;
//     char        pcd;
//     char        pwd;
//     char        user_supervisor;
//     char        read_write;
//     char        present;
// } pdp_entry;
//
// typedef struct {
//     char        nx;
//     char[11]    avl_0;
//     char[40]    address;
//     char[3]     avl_1;
//     char[3]     zero_0;
//     char        accessed;
//     char        pcd; // chache flag
//     char        pwd; // chache flag
//     char        user_supervisor;
//     char        read_write;
//     char        present;
// } pd_entry;
//
// typedef struct {
//     char        nx;
//     char[11]    avl_0;
//     char[40]    address;
//     char[3]     avl_1;
//     char        global;
//     char        pat;
//     char        dirty; // clear to 0
//     char        accessed;
//     char        pcd; // chache flag
//     char        pwd; // chache flag
//     char        user_supervisor;
//     char        read_write;
//     char        present;
// } pt_entry;

// void
// init_paging(void)
// {
//     void * cr3 = pmem_alloc();
//     set_cr3(cr3);
// }
//
// void *
// get_cr3(void)
// {
//
// }
//
// void
// set_cr3(void)
// {
//
// }
//
// void
// get_pml4_address()void
// {
//
// }
//
// void *
// virtual_to_physical(void)
// {
//
// }
//
// void *
// physical_to_virtual(void)
// {
//
// }
