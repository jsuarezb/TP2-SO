#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "task.h"

#define NULL (void*)0

void sched_init();

task_t * get_current_task(void);

task_t* create_task(void* func, int argc, char**argv);

task_t* find_task_with_pid(int pid);

void add_task(task_t* task);
void remove_task_with_pid(int pid);

void pause_task_with_pid(int pid);
void resume_task_with_pid(int pid);

#endif
