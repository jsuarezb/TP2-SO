#ifndef _TASK_H
#define _TASK_H

typedef void* stack_ptr;

#define MAX_TASKS	128
#define STACK_BASE	0x80000000
#define STACK_SIZE	0x10000

typedef struct{
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;

	uint64_t base;
} context_t;

typedef struct task_t task_t;

struct task_t {
	int pid;
	stack_ptr stack;
	task_t* next;
	void* entryPoint;
};

void task_init(task_t* task, void* func, int argc, char** argv);

#endif