
/**
 * Physical memory manager
 */

#include <stdint.h>
#include "pmem.h"

/**
 * Returns the bit index inside the byte of the bitmap
 *
 * @param   address pointer of physical memory address
 * @Return  index of the bit
 */
static int get_bit_index(void * address);

/**
 * Returns the byte index inside the bitmap
 *
 * @param   address pointer of physical memory address
 * @return  index of the byte
 */
static int get_byte_index(void * address);

/**
 * Returns 0 if the block that contains the given address is free,
 * 1 if it's allocated
 *
 * @param address pointer to a physical memory address
 * @return 0 if block is free, 1 if not
 */
static int get_block_status(char * bitmap, void * address);

/**
 * Sets the status of the specified block by `byte_index`
 * and `bit_index` with `status`
 *
 * @param   status status of the block
 * @param   byte_index index of the bitmap byte
 * @param   bit_index index of the bit inside the byte
 */
static void set_block_status(char * bitmap, int status, void * address);

/**
 * Returns the index of a free block inside the bitmap
 *
 * @param bitmap    bitmap to look into
 * @return index of the free block or -1 if there is no free block
 */
static uint64_t get_free_block(char * bitmap);

// Block bitmap;
// bit set to 0 when block is free, set to 1 when block is allocated
static char * bitmap = (char *) 0x10000000;

void
init_pmem()
{
    int i;

    for (i = 0; i < BLOCKS * BITS_IN_BYTE; i++)
        bitmap[i] = 0x00;
}

void
pmem_free(void * address)
{
    address = (void *) (address - PMEM_OFFSET);
    // TODO what should we do here
    if ((uint64_t) address >= MEM_SIZE)
        return;

    int is_free = get_block_status(bitmap, address);

    switch (is_free) {
        case FREE_BLOCK:
            // TODO what should we do here
            break;

        case ALLC_BLOCK:
            set_block_status(bitmap, FREE_BLOCK, address);
            break;
    }
}

void *
pmem_alloc()
{
    uint64_t i = get_free_block(bitmap);
    void * address = (void *) (i * BLOCK_SIZE);

    if (i == -1) {
        // TODO what should we do here
    } else {
        set_block_status(bitmap, ALLC_BLOCK, address);
    }

    return address + PMEM_OFFSET;
}

static int
get_byte_index(void * address)
{
    return (uint64_t) address / (BLOCK_SIZE * BITS_IN_BYTE);
}

static int
get_bit_index(void * address)
{
    return ((uint64_t) address / BLOCK_SIZE) % BITS_IN_BYTE;
}

static int
get_block_status(char * bitmap, void * address)
{
    int byte_index, bit_index;

    byte_index = get_byte_index(address);
    bit_index = get_bit_index(address);

    return (bitmap[byte_index] >> bit_index) & 0x01;
}

static void
set_block_status(char * bitmap, int status, void * address)
{
    char aux = 0xFF;
    int bit_index = get_bit_index(address);
    int byte_index = get_byte_index(address);

    switch (status) {
        case FREE_BLOCK:
            aux &= ~(1 << bit_index); // Every bit set to 1 except the bit_index
            bitmap[byte_index] = bitmap[byte_index] & aux;
            break;

        case ALLC_BLOCK:
            bitmap[byte_index] |= (1 << bit_index);
            break;
    }
}

static uint64_t
get_free_block(char * bitmap)
{
    uint64_t i = 0, j = 0;

    while (i < BLOCKS) {
        if (bitmap[i] != 0xFF) {
            j = 0;

            while (j < BITS_IN_BYTE) {
                if (get_block_status(bitmap, (void *) ((i + j) * BLOCK_SIZE))
                        == FREE_BLOCK)
                    return i + j;

                j++;
            }
        }

        i++;
    }

    return -1;
}
