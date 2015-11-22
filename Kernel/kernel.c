#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>

#include "include/define.h"
#include "include/video.h"
#include "include/paging.h"
#include "include/kalloc.h"
#include "pmem.h"
#include "include/scheduler.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

extern void finalizeSetup();

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();
typedef void* stack_ptr;

static struct IDTEntry* idt = (struct IDTEntry*) 0x0000;
struct KBD keyboard;
stack_ptr kernel_stack;

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	/*kernel_stack = pmem_alloc();
	kernel_stack += 0x1000;	// go to the bottom of the stack*/

	kernel_stack = (uint64_t)&endOfKernel + PageSize * 8 - sizeof(uint64_t);//The size of the stack itself, 32KiB

	/*return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);*/
	return kernel_stack;
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();

    ncPrint("Starting pmem...");
    init_pmem();
    ncPrint(" OK");
    ncNewline();

	return getStackBase();
}

/*
 * Modify an entry in the IDT
 */
void setupIDTentry(int index, uint16_t selector, void (*offset)(), uint8_t access)
{
	idt[index].lowBits = (uint64_t) offset & 0xFFFF;
	idt[index].midBits = ((uint64_t) offset >> 16) & 0xFFFF;
	idt[index].highBits = ((uint64_t) offset >> 32) & 0xFFFFFFFF;

	idt[index].selector = selector;
	idt[index].typeAttr = access;

	idt[index].zero1 = 0;
	idt[index].zero2 = 0;
}

/*
 * Setup the IDT so it can get interruptions
 */
void IDTinitialize()
{
	setupIDTentry(TIMER_TICK, 0x08, &_irq00handler, 0x8E);
	setupIDTentry(KEYBOARD, 0x08, &_irq01handler, 0x8E);
	setupIDTentry(SYSCALL, 0x08, &_int80handler, 0x8E);

	picMasterMask(0xF8);
	picSlaveMask(0xFF);

	_sti();
}

int
set_interruptions(int enable)
{
    uint64_t flags = _asm_get_eflags();
    if (enable)
        _sti();
    else
        _cli();

    return (flags & 0x200) != 0;
}

static int init_task_pid = -1;

void initialize_task() {
	IDTinitialize();
	while(1);
}

void shell_task(){

	// Remove the init_task
	remove_task_with_pid(init_task_pid);

	// Start the shell
	((EntryPoint)sampleCodeModuleAddress)();
}

int main()
{
	setupIDTentry(PAGE_FAULT, 0x08, &_exc14handler, 0x8E);

	init_paging();

    ncPrint("Starting scheduler");
    sched_init();
    ncPrint(" OK ");
    ncNewline();

    ncPrint("Creating shell");
    task_t* shell = create_task((void*)shell_task, 0, NULL);
	add_task(shell);
	ncPrint(" OK ");

	// Task to initialize the IDT
	task_t* init_task = create_task((void*)initialize_task, 0, NULL);
	add_task(init_task);
	init_task_pid = init_task->pid;

	_vClear();

	finalizeSetup();

	return 0;
}
