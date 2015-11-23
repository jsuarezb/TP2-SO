#include <stdint.h>
#include "include/api.h"
#include "include/define.h"
#include "include/handler.h"
#include "include/kalloc.h"
#include "include/keyboard.h"
#include "include/lib.h"
#include "include/screensaver.h"
#include "include/task.h"
#include "include/scheduler.h"
#include "include/shm.h"
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
static int syscall_proc_init(void (*func)(int, char **), int argc, char ** argv);

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
 */
static void syscall_proc_signal(int pid);

/**
 * Yield the CPU to the next process
 */
static void syscall_proc_yield(void);

void
pageFaultHandler(void * address)
{
    int is_enabled = SetInts(0);

    task_t* task = current_task();

    uint64_t top_limit = (uint64_t) task->stack_base;
    uint64_t bottom_limit = (uint64_t) task->stack_base - STACK_SIZE;

    int is_inside = (uint64_t) address <= top_limit
        && (uint64_t) address > bottom_limit;

    // ncPrint("Top limit: "); ncPrintHex(top_limit); ncNewline();
    // ncPrint("Bottom limit: "); ncPrintHex(bottom_limit); ncNewline();
    // ncPrint("Address accessed: "); ncPrintHex(address); ncNewline();
    // ncPrint("RSP: "); ncPrintHex(task->stack); ncNewline();

    if (is_inside) {
        // Give 4kb of stack
        virtual_kalloc(address);
        SetInts(is_enabled);
    } else {
        ncPrintHex(address);
        remove_task_with_pid(task->pid);
        SetInts(is_enabled);

        _reschedule();
    }

}

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
    void * a;

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
            return syscall_proc_init((void *) arg1, arg2, arg3);

        case SYS_PROC_KILL:
            syscall_proc_kill(arg1);
            break;

        case SYS_PROC_SLEP:
            syscall_proc_sleep();
            break;

        case SYS_PROC_SIGN:
            syscall_proc_signal(arg1);
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

        case SYS_SEM_CREAT:
            return create_sem(arg1, arg2);

        case SYS_SEM_DELETE:
            delete_sem(arg1);
            break;

        case SYS_SEM_UP:
            sem_up(arg1);
            break;

        case SYS_SEM_DOWN:
            sem_down(arg1);
            break;

        case SYS_SEM_GET:
            return sem_get(arg1);

        case SYS_SHM_OPEN:
            return shm_open(arg1);

        case SYS_SHM_GET:
            return shm_get(arg1);

        case SYS_SHM_CLOSE:
            return shm_close(arg1);

        case SYS_GIVE_FOREGROUND:
            give_foreground(arg1);
            break;

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
    ps_list * list = kalloc();
    list->list = (ps_entry *) kalloc();
    ps_entry entry;

    task_t * task = get_current_task();
    task_t * fg_task = get_foreground_task();

    int first_pid = task->pid, i = 0;
    do {
        ps_entry * entry = (ps_entry *) kalloc();

        entry->pid = task->pid;
        entry->parent_pid = task->parent_pid;
        entry->foreground = (task->pid == fg_task->pid ? 1 : 0);

        if (task->state == TASK_READY) {
            if (task->pid == first_pid) {
                entry->status = TASK_RUNNING;
            } else {
                entry->status = TASK_READY;
            }
        } else {
            entry->status = TASK_PAUSED;
        }

        list->list[i] = *entry;
        task = task->next;
        i++;
    } while (task->pid != first_pid);

    list->nprocess = i;

    return list;
}

static void *
syscall_ipcs(void)
{
    shm_list * list = kalloc();
    list->addresses = kalloc();

    SharedMemory ** pool = get_shm_pool();
    int i, j = 0;
    for (i = 0; i < MAX_SHM; i++) {
        if (pool[i] == 0)
            continue;

        list->addresses[j] = pool[i];
        j++;
    }

    list->nshm = j;

    return list;
}

static int
syscall_proc_init(void (*func)(int, char **), int argc, char ** argv)
{
    task_t * task = create_task(func, argc, argv);
    if (task == -1)
        return -1;

    add_task(task);

    return task->pid;
}

static void
syscall_proc_kill(int pid)
{
    remove_task_with_pid(pid);
}

static void
syscall_proc_sleep(void)
{
    task_t * task = get_current_task();
    int pid = task->pid;

    pause_task_with_pid(pid);
}

static void
syscall_proc_signal(int pid)
{
    signal_task(pid);
}

static void
syscall_proc_yield(void)
{
    // TODO
}
