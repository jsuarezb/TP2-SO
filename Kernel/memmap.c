
#include <stdint.h>
#include "include/memmap.h"

uint64_t get_free_block(MemoryBitmap * bitmap);

int get_block_status(MemoryBitmap * bitmap, uint64_t byte, int bit);

void set_block_status(MemoryBitmap * bitmap, uint64_t block, int status);

void
init_bitmap(MemoryBitmap * bitmap, char * address, void * offset,
        uint64_t memory_size)
{
    bitmap->address = address;
    bitmap->offset = offset;
    bitmap->size = (memory_size / BLOCK_SIZE / BITS_IN_BYTE) + 1;
    bitmap->last_alloc = 0;

    int i;
    for (i = 0; i < bitmap->size; i++)
        bitmap->address[i] = 0;
}

void *
alloc_block(MemoryBitmap * bitmap)
{
    uint64_t free_block = get_free_block(bitmap);
    if (free_block == -1)
        return 0;

    set_block_status(bitmap, free_block, 1);

    return (void *) ((free_block * BLOCK_SIZE) + (uint64_t) bitmap->offset);
}

void
free_block(MemoryBitmap * bitmap, void * address)
{
    uint64_t relative_address = address - bitmap->offset;
    uint64_t block = relative_address / BLOCK_SIZE;
    if (block >= bitmap->size * BITS_IN_BYTE || block < 0)
        return;

    set_block_status(bitmap, block, 0);
}

uint64_t
get_free_block(MemoryBitmap * bitmap)
{
    int first_search = 1;
    uint64_t byte = bitmap->last_alloc, bit;

    while (byte != bitmap->last_alloc || first_search == 1) {

        if (bitmap->address[byte] != 0xFF) {
            bit = 0;

            while (bit < BITS_IN_BYTE) {
                int status = get_block_status(bitmap, byte, bit);

                if (status == FREE_BLOCK) {
                    bitmap->last_alloc = byte;
                    return byte * BITS_IN_BYTE + bit;
                }

                bit++;
            }
        }

        byte = (byte + 1) % bitmap->size;
        first_search = 0;
    }

    return -1;
}

int
get_block_status(MemoryBitmap * bitmap, uint64_t byte, int bit)
{
    return (bitmap->address[byte] >> bit) & 1;
}

void
set_block_status(MemoryBitmap * bitmap, uint64_t block, int status)
{
    uint64_t byte = block / BITS_IN_BYTE;
    int bit = block % BITS_IN_BYTE;

    bitmap->address[byte] ^= (-status ^ bitmap->address[byte]) & (1 << bit);
}
