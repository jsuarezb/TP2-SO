#ifndef _SEM_H
#define _SEM_H

#include <stdint.h>
#include "task.h"

#define NULL (void*)0
#define MAX_SEM 256

typedef struct{
	int pids[MAX_TASKS];
	uint32_t id;
	uint32_t value;
	uint32_t currentTask;
} sem_t;

sem_t *create_sem(uint32_t id, uint32_t value);

void delete_sem(sem_t *sem);

void sem_up(sem_t *sem);

int sem_down(sem_t *sem);

uint32_t sem_value(sem_t *sem);

sem_t *sem_get(uint32_t id);
#endif
