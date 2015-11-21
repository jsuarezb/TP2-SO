#include <stdint.h>
#include "task.h"
#include "pmem.h"

void task_init(task_t* task, void* func, int argc, char** argv){

	task->stack -= 0x20;	// making space to simulate context switching
	context_t* context = (context_t*)task->stack;
	
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
	context->rsi = 0xB;
	context->rdi = 0xC;
	context->rbp = 0xD;
	context->rdx = 0xE;
	context->rcx = 0xF;
	context->rbx = 0x10;
	context->rax = 0x11;

	context->rip = func;	// we set the rip to the point where the process is
	context->cs = 0x008;	// this 
	context->rflags = 0x202; // and this are copied from other place
	context->rsp = (uint64_t)&(context->base);
	context->ss = 0x0;
	context->base = 0x0;

	task->entryPoint = func;

	task_ready(task);
}

task_t* task_ready(task_t* task){
	task->state = TASK_READY;
}

task_t* task_pause(task_t* task){
	task->state = TASK_PAUSED;
}