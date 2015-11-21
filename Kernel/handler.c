#include <stdint.h>
#include "include/api.h"
#include "include/define.h"
#include "include/handler.h"
#include "include/kalloc.h"
#include "include/keyboard.h"
#include "include/lib.h"
#include "include/screensaver.h"
#include "include/video.h"

extern unsigned int tickCount;
extern unsigned int showingScreensaver;
extern unsigned int timerLimit;
extern unsigned int timer;

/**
 * ALLOC syscall, allocates a block of memory
 *
 * @return the address of the block allocated
 */
static uint64_t syscall_kalloc(void);

/**
 * FREE syscall, free a block of memory
 *
 * @param address   address to free
 */
static void syscall_free(void * address);

/**
 * PS syscall, returns the process list
 *
 * @return a pointer to the list of processes
 */
static void * syscall_process_status(void);

/**
 * IPCS syscall, returns the ipcs list
 *
 * @return a pointer to the list of ipcs
 */
static void * syscall_ipcs(void);

/**
 * Returns the new process pid
 *
 * @return pid of the new process
 */
static int syscall_proc_init(void);

/**
 * Kills a process
 *
 * @param pid   pid of the process to kill
 */
static void syscall_proc_kill(int pid);

/**
 * Sleep the current process
 */
static void syscall_proc_sleep(void);

/**
 * Sends a signal to a process and wake it up if it's sleeping
 *
 * @param pid       pid of the process to signal
 * @param signal    signal to send
 */
static void syscall_proc_signal(int pid, int signal);

/**
 * Yield the CPU to the next process
 */
static void syscall_proc_yield(void);

/*
 * Function to run on timer tick interruption
 */
void
timertickHandler()
{
	if (timer == 0) { // Screensaver state
		if (!showingScreensaver) {
			// If it's not showing the screensaver yet
			startScreensaver();
		} else {
			// If it's already showing it
			tickScreensaver();
		}
	} else {
		timer--;
	}
}

/*
 * Function to run on keyboard interruption
 */
void keyboardHandler(unsigned char c)
{
	timer = timerLimit;
	if (showingScreensaver) {
		stopScreensaver();
	}

	setKey(c);
}

/*
 * Function to run on software interruption (int 80h)
 */
uint64_t
syscallHandler(uint64_t code, uint64_t arg1, uint64_t arg2, uint64_t arg3)
{
	switch (code) {
		case SYS_READ:
			read((unsigned int) arg1, (char *) arg2, (int) arg3);
            break;

		case SYS_WRITE:
			write((unsigned int) arg1, (char *) arg2, (int) arg3);
            break;

		case SYS_TIME:
			getRTC((date *) arg1);
            break;

		case SYS_STIME:
			setRTC((date *) arg1);
            break;

		case SYS_SCREENSAVER:
			setScreensaverTime((int) arg1);
            break;

		case SYS_CPUVENDOR:
			cpuVendor((char *) arg1);
            break;

        case SYS_PROC_INIT:
            return syscall_proc_init();

        case SYS_PROC_KILL:
            syscall_proc_kill(arg1);
            break;

        case SYS_PROC_SLEP:
            syscall_proc_sleep();
            break;

        case SYS_PROC_SIGN:
            syscall_proc_signal(arg1, arg2);
            break;

        case SYS_PROC_YIELD:
            syscall_proc_yield();
            break;

        case SYS_ALLOC:
            return syscall_kalloc();

        case SYS_FREE:
            syscall_free((void *) arg1);
            break;

        case SYS_PS:
            return syscall_process_status();

        case SYS_IPCS:
            return syscall_ipcs();

		default:
			break;
	}

    return 0;
}

static uint64_t
syscall_kalloc(void)
{
    return kalloc();
}

static void
syscall_free(void * address)
{
    kfree(address);
}

static void *
syscall_process_status(void)
{
    void * address = kalloc();

    // TODO fill the address with details of processes

    return address;
}

static void *
syscall_ipcs(void)
{
    void * address = kalloc();

    // TODO fill the address with details of ipcs

    return address;
}

static int
syscall_proc_init(void)
{
    // TODO
    return 0;
}

static void
syscall_proc_kill(int pid)
{
    // TODO
}

static void
syscall_proc_sleep(void)
{
    // TODO
}

static void
syscall_proc_signal(int pid, int signal)
{
    // TODO
}

static void
syscall_proc_yield(void)
{
    // TODO
}
