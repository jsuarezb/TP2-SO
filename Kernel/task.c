#include <stdint.h>
#include "task.h"
#include "pmem.h"

void task_init(task_t* task, void* func, int argc, char** argv){

	task->stack -= 0x20;	// making space to simulate context switching
	context_t* context = (context_t*)task->stack;
	
	context->r15 = 0xA;
	context->r14 = 0x2;
	context->r13 = 0x3;
	context->r12 = 0x4;
	context->r11 = 0x5;
	context->r10 = 0x6;
	context->r9 = 0x7;
	context->r8 = 0x8;
	context->rsi = 0x9;
	context->rdi = 0xA;
	context->rbp = 0xB;
	context->rdx = 0xC;
	context->rcx = 0xD;
	context->rbx = 0xE;
	context->rax = 0xF;

	context->rip = func;	// we set the rip to the point where the process is
	context->cs = 0x008;	// this 
	context->rflags = 0x202; // and this are copied from other place
	context->rsp = task->stack;
	context->ss = 0x0;
	context->base = 0x0;
}