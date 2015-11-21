
#include <stdint.h>

#include "../include/define.h"
#include "../include/lib.h"
#include "../include/shell.h"

char shellBuffer[COMMAND_LINE_SIZE] = {0};
int bufferIndex = 0;

/*
 * Clears the shell buffer
 */
static void clearBuffer();

/*
 * Help function
 */
static void help();

/*
 * Sets the system time
 */
static void setTime();

/*
 * Prints system time
 */
static void getTime();

/*
 * Set screensaver wait time
 */
static void setScreensaver(int seconds);

static void getCpuVendor();

void start_shell()
{
	char c;

	printLineStart();
	// Main loop
	while (1) {
		// Keep waiting till there's a key to read
		c = getChar();

		switch (c) {
			case '\n':
				putChar(c);
				parseCommand(shellBuffer);
				printLineStart();
				break;
			case '\b':
				if (bufferIndex > 0) {
					putChar(c);
					shellBuffer[bufferIndex] = '\0';
					bufferIndex--;
				} else {
					bufferIndex = 0;
					shellBuffer[0] = '\0';
				}

				break;
			default:
				if (bufferIndex == COMMAND_LINE_SIZE - 1) {
					putChar('\b');
					bufferIndex--;
				}

				shellBuffer[bufferIndex++] = c;
				putChar(c);
				break;
		}
	}
}

void printLineStart()
{
	printf("#> ");
}

void parseCommand(const char * line)
{
	char command[80] = {0};
	char args[80] = {0};

	// We need at least the command to be scanned
	int hasCommand = sscanf(line, "%s %s", command, args);
	if (hasCommand == 0)
		return;

	if (strcmp(command, HELP_COMMAND) == 0) {
		help();
	} else if (strcmp(command, SET_TIME_COMMAND) == 0) {
		setTime();
	} else if (strcmp(command, GET_TIME_COMMAND) == 0) {
		getTime();
	} else if (strcmp(command, SET_SCREENSAVER_COMMAND) == 0) {
		int seconds = 0;
		if (sscanf(args, "%d", &seconds) > 0 && seconds > 0) {
			setScreensaver(seconds);
		} else
			printf("Invalid argument\n");
	} else if (strcmp(command, GET_CPU_VENDOR_COMMAND) == 0) {
		getCpuVendor();
	} else if (strcmp(command, ALLOC_COMMAND) == 0) {
        void * add = malloc();
        printf("Address allocated: %x\n", (uint64_t) add);
        free(add);

        add = malloc();
        printf("Address allocated: %x\n", (uint64_t) add);
    } else {
		printf("Command not found.\n");
	}

	clearBuffer();
}

static void clearBuffer(void)
{
	int i;
	for (i = 0; i < 76; i++)
		shellBuffer[i] = 0;

	bufferIndex = 0;
}

/*
 * Shell functions
 */
static void help()
{
	int opt = 0;
	printf("Hi, I'm help, available commands:\n");
	printf("Please select your option\n");
	printf("0 - GET TIME\n");
	printf("1 - SET TIME\n");
	printf("2 - SET SCREENSAVER TIME\n");
	printf("3 - GET CPU VENDOR\n");

	if (scanf("%d", &opt) == 0 || opt > 3) {
		printf("Invalid option\n");
		return;
	}

	switch(opt){
		case GET_DATE:
			printf("GET DATE:\n");
			printf("Command: time\n");
			printf("Display the current date and time in the following format: YYYY/MM/DD HH:MM:SS\n");
			break;
		case SET_DATE:
			printf("SET DATE:\n");
			printf("Command: stime\n");
			printf("Set the system current date and time\n");
			break;
		case SET_SCREENSAVER:
			printf("SET SCREENSAVER TIME:\n");
			printf("Command: ss n\n");
			printf("** n is the timeout period in seconds **\n");
			printf("Set the scrensaver timeout period\n");
			break;
		case GET_CPU_VENDOR:
			printf("GET CPU VENDOR:\n");
			printf("Command: cpuid\n");
			printf("Display the CPU's manufacturer ID string\n");
			break;
		default:
			printf("Invalid command.\n");
	}
}

static void printStruct ( date * current_date ){
	printf("Seconds: %d\n", current_date->second);
	printf("Minutes: %d\n", current_date->minute);
	printf("Hours: %d\n", current_date->hour);
	printf("Day: \n", current_date->day);
	printf("Month: \n", current_date->month);
	printf("Year: %d\n", current_date->year);
}

static void setTime()
{
	int hour = 0, minute = 0, second = 0;
	int year = 1, day = 0, month = 0;
	date current_date;

	printf("Enter date in this format: YY MM DD HH MM SS\n");
	do {
		scanf("%d %d %d %d %d %d", &year, &month, &day, &hour, &minute, &second);
		if (year <= 0  || year > 99 || month <= 0 || day <= 0 || day > 31 || month > 12 || hour < 0 || hour > 23 || minute < 0 || minute > 50 || second < 0 || second > 59){
			printf("Invalid Format\n");
		}
	} while ( year <= 0 || year > 99 || month <= 0 || day <= 0 || day > 31 || month > 12 || hour < 0 || hour > 23 || minute < 0 || minute > 50 || second < 0 || second > 59);

	printf("day: %d\n", day);
	current_date.hour = hour;
	current_date.minute = minute;
	current_date.second = second;
	current_date.day = day;
	current_date.month = month;
	current_date.year = year;

	printStruct(&current_date);

	_sys_call(SYS_STIME, (uint64_t) &current_date, 1, 1);
	printf("Complete.\n");
	getTime();
}

static void getTime()
{
	date current_date;
	_sys_call(SYS_TIME, (uint64_t) &current_date, 1, 1);
	printf(
		"Current date and time: %d/%d/%d %d:%d:%d\n",
		current_date.year,
		current_date.month,
		current_date.day,
		current_date.hour,
		current_date.minute,
		current_date.second
	);
}

static void setScreensaver(int seconds)
{
	_sys_call(SYS_SCREENSAVER, seconds, 0, 0);
	printf("Screensaver timeout set to %d seconds\n", seconds);
}

static void getCpuVendor()
{
	char vendor[13];
	_sys_call(SYS_CPUVENDOR, (uint64_t) vendor, 0, 0);
	vendor[12] = '\0';
	printf("%s\n", vendor);
}
