#include <stdint.h>
#include "task.h"
#include "pmem.h"

task_t *task_create(EntryPoint func, int pid, int argc, char** argv){

	task_t* newTask = pmem_alloc();	// Task allocated with 4k
	context_t* context;

	newTask->pid = pid;
	newTask->stack = pmem_alloc(); // Stack of task allocated with 4k
	newTask->stack += 0x1000 - 0xF; // Move the pointer to the bottom of the stack and make room for some context
	context = newTask->stack;	// Fill the space with the context, the stack is pointing to the first variable of the context r15

	context->r15 = 0x0;
	context->r14 = 0x0;
	context->r13 = 0x0;
	context->r12 = 0x0;
	context->r11 = 0x0;
	context->r10 = 0x0;
	context->r9 = 0x0;
	context->r8 = 0x0;
	context->rsi = 0xB;
	context->rdi = 0xE;
	context->rbp = 0x0;
	context->rdx = 0x0;
	context->rcx = 0x0;
	context->rbx = 0x0;
	context->rax = 0x0;
	context->rip = func;	// we set the rip to the point where the process is
	context->cs = 0x008;	// this 
	context->rflags = 0x202; // and this are copied from other place
	context->rsp = (&context->base);
	context->cs = 0x0;
	context->base = 0x0;

	return newTask;
}