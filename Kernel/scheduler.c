#include <stdint.h>
#include "scheduler.h"
#include "include/lib.h"
#include <naiveConsole.h>
#include "video.h"

static int current_pid = -1;
task_t* current;

stack_ptr stacks[MAX_TASKS];
task_t tasks[MAX_TASKS];

extern stack_ptr kernel_stack;

static int next_pid(){

	current_pid++;

	if(current_pid == MAX_TASKS)
		return -1;

	return current_pid;
}

/**
 * Initializes the scheduler
 */

void sched_init() {

	uint64_t stack_base = STACK_BASE;

	for(int i = 0; i<MAX_TASKS; i++){
		stacks[i] = (stack_ptr)(stack_base + STACK_SIZE*i);
	}

}

/**
 * Returns the current task
 */
task_t *
get_current_task(void)
{
    return current;
}

/**
 * Schedules the first available task that's ready
 */

void schedule()
{

    int i = SetInts(0);

	current = current->next;

	while(current->state == TASK_PAUSED)
		current = current->next;

    SetInts(i);
}

/**
 * Finds a task with the pid
 *
 * @params pid Process id of the task you want to find
 *
 * return the task with the pid, -1 if not found
 */

task_t* find_task_with_pid(int pid){

	int inital_pid = current->pid;

	task_t* t = current;

	while(t->pid != pid){
		t = t->next;

		if(t->pid == inital_pid){
			t = -1;
			break;
		}
	}

	return t;
}

/**
 * Pauses the task with pid
 *
 * @params pid Process id of the task you pause
 *
 */

void pause_task_with_pid(int pid){

    int i = SetInts(0);

	task_t* task = find_task_with_pid(pid);

	if(task != -1)
		task_pause(task);

    SetInts(i);
}

/**
 * Resume the task with pid
 *
 * @params pid Process id of the task you resume
 *
 */

void resume_task_with_pid(int pid){

    int i = SetInts(0);

	task_t* task = find_task_with_pid(pid);

	if(task != -1)
		task_ready(task);

    SetInts(i);
}

/**
 * Adds a task to the list of running tasks
 *
 * @params task to add to the list
 *
 */

void add_task(task_t* task){

    int i = SetInts(0);

	if(current == 0){
		current = task;
		current->next = current;
	} else  {
		task_t* tmp = current->next;
		current->next = task;
		task->next = tmp;
	}

    SetInts(i);

}

/**
 * Removes task from list
 *
 * @params pid of task to remove from the list
 *
 */

void remove_task_with_pid(int pid){

    int i = SetInts(0);

	task_t* tr = find_task_with_pid(pid);

	if (tr != -1) {
    	task_t* pr = tr->next;

    	while(pr->next->pid != pid)
    		pr = pr->next;

    	pr->next = tr->next;
    }

    SetInts(i);
}

/**
 * Creates a new task
 *
 * @params func task code location
 * @params argc
 * @params argv
 *
 * return new task
 */

task_t* create_task(void (*func)(int, char **), int argc, char**argv){

    int i = SetInts(0);

	int pid = next_pid();

    task_t * task;

	if (pid != -1) {
    	task = &tasks[pid];
    	task->next = 0;
    	task->pid = pid;
    	task->stack = stacks[pid] + STACK_SIZE;

    	task_init(task, func, argc, argv);
    }

    SetInts(i);

	return pid == -1 ? -1 : task;
}


/**
 * Sends a signal to a process
 *
 * @param pid   pid of the process
 */
void
signal_task(int pid)
{
    task_t * task = find_task_with_pid(pid);
    resume_task_with_pid(pid);

    return;
}

/*
 * asm functions
 */

stack_ptr switch_user_to_kernel(stack_ptr esp) {

    int i = SetInts(0);
	current->stack = esp;
    SetInts(i);

	return kernel_stack;
}

stack_ptr switch_kernel_to_user(stack_ptr esp) {

	schedule();

	return current->stack;
}

stack_ptr get_entry_point(){
	return current->entryPoint;
}
