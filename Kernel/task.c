#include <stdint.h>
#include "include/task.h"
#include "include/kalloc.h"
#include "include/pmem.h"
#include "include/paging.h"
#include "include/lib.h"
#include "include/scheduler.h"

void task_init(task_t* task, void (*func)(int, char **), int argc, char ** argv){

    int i = SetInts(0);

	task->stack -= sizeof(context_t);	// making space to simulate context switching
	context_t* context = (context_t*)task->stack;

    virtual_kalloc(task->stack);

	context->gs = 0x01;
	context->fs = 0x02;
	context->r15 = 0x03;
	context->r14 = 0x04;
	context->r13 = 0x05;
	context->r12 = 0x06;
	context->r11 = 0x07;
	context->r10 = 0x08;
	context->r9 = 0x09;
	context->r8 = 0xA;
	context->rsi = (uint64_t) argc;
	context->rdi = (uint64_t) func;
	context->rbp = 0xD;
	context->rdx = (uint64_t) argv;
	context->rcx = 0xF;
	context->rbx = 0x10;
	context->rax = 0x11;

	context->rip = (uint64_t) task_wrapper;	// we set the rip to the point where the process is
	context->cs = 0x008;	// this
	context->rflags = 0x202; // and this are copied from other place
	context->rsp = (uint64_t) task->stack;

	context->ss = 0x0;
	context->base = 0x0;

	task->entryPoint = func;

	task_ready(task);

    SetInts(i);
}

void
task_wrapper(void (*func)(int, char **), int argc, char ** argv)
{
    func(argc, argv);

    int i = SetInts(0);

    task_t * task = get_current_task();

    give_foreground(task->parent_pid);
    remove_task_with_pid(task->pid);

    SetInts(i);
    _reschedule();
}

task_t*
task_ready(task_t* task){
	task->state = TASK_READY;

    return task;
}

task_t*
task_pause(task_t* task){
	task->state = TASK_PAUSED;

    return task;
}
