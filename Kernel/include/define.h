#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <stdint.h>

#define FALSE	0
#define TRUE	!FALSE

// Date
#define SECOND 			0x00      
#define MINUTE 			0x02
#define HOUR 			0x04
#define DAY 			0x07
#define MONTH 			0x08
#define YEAR 			0x09

// Video driver
#define VIDEO_START		0xB8000
#define VIDEO_WIDTH		80
#define VIDEO_HEIGHT	25

#define WHITE_BG	0xF0
#define WHITE_FG	0x0F
#define RED_BG		0xC0
#define RED_FG		0x0C
#define BLACK_BG	0x00
#define BLACK_FG	0x00

#define DEFAULT_CONSOLE_FORMAT	BLACK_BG | WHITE_FG
#define ERROR_CONSOLE_FORMAT	RED_BG | WHITE_FG

// Screensaver
#define FRAMES	3

// Keyboard driver
#define ASCII_ESCAPE	27
#define ASCII_CTRL		29
#define ASCII_SHIFT		20
#define ASCII_PRTSC		15
#define ASCII_ALT		27
#define ASCII_CAPS		27
#define ASCII_F1		27
#define ASCII_F2		27
#define ASCII_F3		27
#define ASCII_F4		27
#define ASCII_F5		27
#define ASCII_F6		27
#define ASCII_F7		27
#define ASCII_F8		27
#define ASCII_F9		27
#define ASCII_F10		27
#define ASCII_NUMLOCK	27
#define ASCII_SCROLLLOCK	27
#define ASCII_DELETE	127
#define SYS_REQ			27

#define RELEASE_FLAG	0x80

#define LSHIFT_PRESSED	42
#define LSHIFT_RELEASED LSHIFT_PRESSED | RELEASE_FLAG
#define RSHIFT_PRESSED	54
#define RSHIFT_RELEASED RSHIFT_PRESSED | RELEASE_FLAG
#define CAPS_PRESSED	58
#define CAPS_RELEASED	CAPS_PRESSED | RELEASE_FLAG
#define CTRL_PRESSED	29
#define CTRL_RELEASED	29 | RELEASE_FLAG 
#define ALT_PRESSED		56
#define ALT_RELEASED	56 | RELEASE_FLAG

#define KBD_BUFFER_SIZE	16

#define UPPERCASE	0
#define LOWERCASE	1

// System calls
#define SYS_READ	0x03
#define SYS_WRITE	0x04
#define SYS_TIME	0x0D
#define SYS_STIME	0x19
#define SYS_SCREENSAVER	0x20
#define SYS_CPUVENDOR 0X24

#define STDIN	0
#define STDOUT	1

// IDT
/*
 * IDTR and IDT entry structure taken from
 * http://wiki.osdev.org/Interrupt_Descriptor_Table#IDT_in_IA-32e_Mode_.2864-bit_IDT.29
 */
#define IDT_OFFSET	0x20 // Pure64 interruptions offset
#define TIMER_TICK	IDT_OFFSET + 0x00
#define KEYBOARD	IDT_OFFSET + 0x01
#define SYSCALL		0x80

/*
 * Register that points to the IDT
 * 0..15	size of the IDT
 * 16..63	memory offset of the IDT
 */
struct IDTR {
	uint16_t limit;
	uint64_t offset;
};

/*
 * Structure of an IDT Descriptor
 * 0..15	offset low bits
 * 16..31	selector
 * 32..39	zeros
 * 40..47	type and attributes
 * 48..55	offset middle bits
 * 56..71	offset high bits
 * 72..95	zeros
 */
struct IDTEntry {
	uint16_t lowBits;
	uint16_t selector;
	uint8_t zero1;
	uint8_t typeAttr;
	uint16_t midBits;
	uint32_t highBits;
	uint32_t zero2;
};

struct KBD {
	int capsEnabled;
	int shiftEnabled;
	int altEnabled;
	int ctrlEnabled;
	int readIndex;
	int writeIndex;
	int buffer[KBD_BUFFER_SIZE];
};

struct KBDKey {
	int shown;
	unsigned char asciiCode;
	unsigned char keyCode;
};

struct ScreenBackup {
	char video[VIDEO_WIDTH * VIDEO_HEIGHT * 2];
	int cursor;
};

typedef struct date {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
   	uint8_t minute;
   	uint8_t second;
} date;

#endif
