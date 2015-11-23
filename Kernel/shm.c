

#include "include/kalloc.h"
#include "include/shm.h"

static SharedMemory ** shm_pool = 0xA00000;

static int get_free_index(void);

void
init_shm()
{
    int i;
    for (i = 0; i < MAX_SHM; i++)
        shm_pool[i] = 0;
}

void *
shm_open(int key)
{
    int i = get_free_index();

    if (i == -1)
        return 0;

    shm_pool[i] = kalloc();
    shm_pool[i]->start = kalloc();
    shm_pool[i]->key = key;

    return shm_pool[i]->start;
}

void *
shm_get(int key)
{
    int i;
    for (i = 0; i < MAX_SHM; i++) {
        if (shm_pool[i] != 0 && shm_pool[i]->key == key) {
            return shm_pool[i]->start;
        }
    }

    return 0;
}

int
shm_close(int key)
{
    int i;
    for (i = 0; i < MAX_SHM; i++) {
        if (shm_pool[i] != 0) {
            if (shm_pool[i]->key == key) {
                kfree(shm_pool[i]->start);
                kfree(shm_pool[i]);

                return 1;
            }
        }
    }

    return 0;
}

static int
get_free_index(void)
{
    int i;
    for (i = 0; i < MAX_SHM; i++)
        if (shm_pool[i] == 0)
            return i;

    return -1;
}
