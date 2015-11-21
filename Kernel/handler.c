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

static uint64_t syscall_kalloc(void);

static void syscall_free(void * address);

/*
 * Function to run on timer tick interruption
 */
void timertickHandler()
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

        case SYS_ALLOC:
            return syscall_kalloc();

        case SYS_FREE:
            syscall_free((void *) arg1);
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
