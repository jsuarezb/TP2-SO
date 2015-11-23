#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "task.h"

void sched_init();

task_t * get_current_task(void);
task_t * get_foreground_task(void);

task_t* create_task(void (*func)(int, char **), int argc, char**argv);

task_t* find_task_with_pid(int pid);
task_t* current_task();

void add_task(task_t* task);
void remove_task_with_pid(int pid);

void pause_task_with_pid(int pid);
void resume_task_with_pid(int pid);

void give_foreground(int pid);

void set_waiting_foreground(int pid, int status);

int is_waiting_foreground(int pid);

#endif
