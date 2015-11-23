#ifndef _DEFINE_H
#define _DEFINE_H

#include <stdint.h>

#define SYS_READ	0x03
#define SYS_WRITE	0x04
#define SYS_TIME	0x0D
#define SYS_STIME	0x19
#define SYS_SCREENSAVER	0x20
#define SYS_CPUVENDOR	0x24
#define SYS_PROC_INIT   0x30
#define SYS_PROC_KILL   0x31
#define SYS_PROC_SLEP   0x32
#define SYS_PROC_SIGN   0x33
#define SYS_PROC_YIELD  0x34
#define SYS_ALLOC   0x5A
#define SYS_FREE    0x5B
#define SYS_PS      0x60
#define SYS_IPCS    0x61
#define SYS_SEM_CREAT   0x70
#define SYS_SEM_DELETE  0x71
#define SYS_SEM_UP      0x72
#define SYS_SEM_DOWN    0x73
#define SYS_SEM_GET     0x74
#define SYS_SHM_OPEN    0x75
#define SYS_SHM_GET     0x76
#define SYS_SHM_CLOSE   0x77

#define COMMAND_LINE_SIZE	77
#define GET_DATE			0
#define SET_DATE			1
#define SET_SCREENSAVER		2
#define GET_CPU_VENDOR		3

#define STDIN	0
#define STDOUT	1

#define HELP_COMMAND	"help"
#define SET_TIME_COMMAND	"stime"
#define GET_TIME_COMMAND	"time"
#define SET_SCREENSAVER_COMMAND	"ss"
#define GET_CPU_VENDOR_COMMAND	"cpuid"
#define ALLOC_COMMAND       	"alloc"
#define SEM_1       	"sem1"
#define SEM_2       	"sem2"

typedef struct date {
	uint8_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
   	uint8_t minute;
   	uint8_t second;
} date;

#endif
