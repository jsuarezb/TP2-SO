#include "include/pmem.h"
#include "include/sem.h"
#include "include/scheduler.h"
#include "include/define.h"

uint32_t currentId = 0;
static sem_t *sem_array[MAX_SEM];

sem_t *
create_sem(uint32_t id, uint32_t value)
{
    sem_t * sem_aux = sem_get(id);
    if (sem_aux != NULL)
        return NULL;

	sem_t *sem = pmem_alloc();

	sem->id = id;
	sem->value = value;
	sem->currentTask = 0;

	if(currentId>MAX_SEM){
		return NULL;
	}

	sem_array[currentId++] = sem;

	return sem;
}

void
delete_sem(sem_t *sem){
	pmem_free(sem);
}

void
sem_up(sem_t *sem){
	if (sem->currentTask == 0) {
		sem->value++;
    } else {
    	resume_task_with_pid(sem->pids[0]);
    	for(int i = 0; i < sem->currentTask; i++)
    		sem->pids[i] = sem->pids[i+1];

    	sem->currentTask--;
    }
}

int
sem_down(sem_t *sem){
	if(sem->value > 0){
		sem->value--;
		return TRUE;
	}

    task_t * task = get_current_task();
    int pid = task->pid;
	sem->pids[sem->currentTask++] = pid;

	pause_task_with_pid(pid);

	return FALSE;
}

uint32_t
sem_value(sem_t *sem){
	return sem->value;
}

sem_t *
sem_get(uint32_t id){
	for(int i =0; i<currentId; i++){
		if(sem_array[i] != NULL){
			if(sem_array[i]->id ==id){
				return sem_array[i];
			}
		}
	}
	return NULL;
}
