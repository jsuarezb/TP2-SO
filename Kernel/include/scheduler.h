#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "task.h"

#define NULL (void*)0

static int next_pid();
void sched_init();
stack_ptr sched_switch_to_kernel(stack_ptr stack);
stack_ptr sched_switch_to_user();

#endif