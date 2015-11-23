
#ifndef SHM_H
#define SHM_H

#define MAX_SHM 128

typedef struct SharedMemory {
    void * start;
    int key;
} SharedMemory;

void init_shm();

void * shm_open(int key);

void * shm_get(int key);

int shm_close(int key);

#endif
