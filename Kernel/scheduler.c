#include <stdint.h>
#include "scheduler.h"
#include "include/lib.h"

static int current_pid = 1;
static task_t* current = NULL;

extern stack_ptr kernel_stack;

static int next_pid(){
	return current_pid++;
}

void sched_init() {

	_cli();

	// Creating shell
	task_t *shell_task = task_create((EntryPoint)0x400000, next_pid(), 0, NULL);
	// Adding to scheduler shell
	sched_schedule(shell_task);

	_sti();
}

void sched_schedule(task_t* task){
	if(current == NULL){
		current = task;
	} else {
		// TODO not first task
	}
}

stack_ptr sched_switch_to_kernel(stack_ptr stack) {
	return kernel_stack;
}

stack_ptr sched_switch_to_user() {
	// TODO return the stack of the next process
	return current->stack;
}