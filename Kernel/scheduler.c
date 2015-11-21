#include <stdint.h>
#include "scheduler.h"
#include "include/lib.h"
#include <naiveConsole.h>
#include "video.h"

static int current_pid = 0;
task_t* current;

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
	task_t* task = &tasks[pid];
	task->next = 0;
	task->pid = pid;
	task->entryPoint = func;
	task->stack = stacks[pid] + STACK_SIZE;
	task->state = TASK_UNAVAILABLE;

	task_init(task, func, argc, argv);

	return task;
}

task_t* task_ready(task_t* task){
	task->state = TASK_READY;
}

task_t* task_pause(task_t* task){
	task->state = TASK_PAUSED;
}

task_t* task_current(task_t* task){
	task->state = TASK_CURRENT;
}

void sched_init() {

	uint64_t stack_base = STACK_BASE;

	for(int i = 0; i<MAX_TASKS; i++){
		stacks[i] = (stack_ptr)(stack_base + STACK_SIZE*i);
	}

}

stack_ptr switch_user_to_kernel(stack_ptr esp) {

	current->stack = esp;

/*	ncNewline();
	ncPrint("u2k");
	ncPrint("  ");
	ncPrintHex(esp);
	ncPrint("  ");
	ncPrintDec(current->pid);
	ncNewline(); */

	return kernel_stack;
}

stack_ptr switch_kernel_to_user(stack_ptr esp) {

	schedule();

/*	ncPrint("k2u");
	ncPrint("  ");
	ncPrintHex(current->stack);
	ncPrint("  ");
	ncPrintDec(current->pid);
	ncNewline(); */

	return current->stack;
}


stack_ptr get_entry_point(){
	return current->entryPoint;
}