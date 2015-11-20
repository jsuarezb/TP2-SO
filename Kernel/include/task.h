#ifndef _TASK_H
#define _TASK_H

typedef int (*EntryPoint)();
typedef void* stack_ptr;

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

typedef enum{
	TASK_READY,
	TASK_PAUSED,
	TASK_CURRENT
} task_state_t;

typedef enum{
	TASK_FOREGROUND,
	TASK_BACKGROUND
} task_mode_t;

typedef struct {
	task_state_t state;
	stack_ptr stack;

 	struct task_t *next;
	struct task_t *prev;

 	int pid;
 } task_t;

task_t *task_create(EntryPoint func, int pid, int argc, char** argv);

#endif