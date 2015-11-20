#include <stdint.h>
#include "scheduler.h"
#include "include/lib.h"
#include <naiveConsole.h>

static int current_pid = 0;
task_t* current = 0;
int isKernel = 1;

stack_ptr stacks[MAX_TASKS];
task_t tasks[MAX_TASKS];

extern stack_ptr kernel_stack;

static int next_pid(){
	return current_pid++;
}

void schedule(){
	current = current->next;
}

void add_task(task_t* task){

	if(current == 0){
		current = task;
		current->next = current;
	} else  {
		task_t* tmp = current->next;
		current->next = task;
		task->next = tmp;
	}

}

task_t* create_task(void* func, int argc, char**argv){

	int pid = next_pid();
	task_t task = tasks[pid];
	task.next = 0;
	task.pid = pid;
	task.entryPoint = func;
	task.stack = stacks[pid] + STACK_SIZE;

	task_init(&task, func, argc, argv);

	return &task;
}

void sched_init() {

	_cli();

	uint64_t stack_base = STACK_BASE;

	for(int i = 0; i<MAX_TASKS; i++){
		stacks[i] = (stack_ptr)(stack_base + STACK_SIZE*i);
	}

	_sti();
}

stack_ptr switch_user_to_kernel(stack_ptr esp) {

	current->stack = esp;

	return kernel_stack;
}

stack_ptr switch_kernel_to_user(stack_ptr old_stack) {

	schedule();

	return current->stack;
}


stack_ptr get_entry_point(){
	return current->entryPoint;
}