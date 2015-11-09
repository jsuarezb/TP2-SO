#ifndef _PAGING_H
#define _PAGING_H

// A long mode virtual address is mapped as follows:
// Most significant 9 bits -> PML4 index
// Next 9 bits -> PDP index
// Next 9 bits -> PD index
// Next 9 bits -> PT index
// Last 12 bits -> index inside the page

#endif
