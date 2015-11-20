#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "task.h"

#define NULL (void*)0

static int next_pid();

void sched_init();

stack_ptr switch_user_to_kernel(stack_ptr esp);
stack_ptr switch_kernel_to_user(stack_ptr esp);

stack_ptr get_entry_point();

#endif