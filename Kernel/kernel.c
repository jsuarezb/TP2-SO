#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>

#include "include/define.h"
#include "include/video.h"
#include "pmem.h"

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

static struct IDTEntry* idt = (struct IDTEntry*) 0x0000;
struct KBD keyboard;

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
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

int main()
{
    IDTinitialize();
    init_pmem();

    _vClear();
    ((EntryPoint)sampleCodeModuleAddress)();

	return 0;
}
