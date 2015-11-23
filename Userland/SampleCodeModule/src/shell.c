
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

void alloc_main(int argc, char ** argv);

void main_test_1_ipc(int argc, char ** argv);

void main_test_2_ipc(int argc, char ** argv);

void main_test_3_ipc(int argc, char ** argv);

void main_ps(int argc, char ** argv);

void main_ipcs(int argc, char ** argv);

void parse_sh(const char * command, int fg);

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
	} else if (strcmp(command, SH_COMMAND) == 0) {
        parse_sh(line, 0);
    } else if (strcmp(command, SHF_COMMAND) == 0) {
        parse_sh(line, 1);
    } else if (strcmp(command, PS_COMMAND) == 0) {
        init_proc(main_ps, 0, 0);
    } else if (strcmp(command, IPCS_COMMAND) == 0) {
        init_proc(main_ipcs, 0, 0);
    } else {
		printf("Command not found.\n");
	}

	clearBuffer();
}

static void
clearBuffer(void)
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
	printf("0 - sh\n");
	printf("1 - sem1\n");
	printf("2 - sem2\n");
    printf("3 - sem3\n");
    printf("4 - ps\n");
	printf("5 - ipcs\n");
    printf("6 - help\n");

	if (scanf("%d", &opt) == 0 || opt > 6) {
		printf("Invalid option\n");
		return;
	}

	switch(opt){
		case 0:
			printf("sh [command]:\n");
			printf("executes a process\n");
			break;
		case 1:
			printf("sem1:\n");
			printf("create a semaphore with key 1 and reads from a shared memory\n");
			break;
		case 2:
			printf("sem2:\n");
			printf("ups a semaphore with key 1 and writes in a shared memory\n");
			break;
		case 3:
			printf("sem3:\n");
			printf("reads from a shared memory\n");
			break;
		case 4:
			printf("ps:\n");
			printf("show active processes\n");
			break;
		case 5:
			printf("ipcs:\n");
			printf("show active ipcs (shared memories created)\n");
			break;
		case 6:
			printf("help:\n");
			printf("show help\n");
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

void
parse_sh(const char * command, int fg)
{
    char sh[77];
    char process_name[77];
    char arg2[77];
    int i = sscanf(command, "%s %s %s", sh, process_name, arg2);

    char * argv[77];
    argv[0] = process_name;
    argv[1] = arg2;

    void (*func)(int, char **);

    if (strcmp(process_name, SEM_1) == 0) {
        func = main_test_1_ipc;
    } else if (strcmp(process_name, SEM_2) == 0) {
        func = main_test_2_ipc;
    } else if (strcmp(process_name, SEM_3) == 0) {
        func = main_test_3_ipc;
    } else {
        printf("No existe el proceso\n");
        return;
    }

    int pid = init_proc(func, i - 1, argv);
    if (pid == -1) {
        printf("No se pudo crear el proceso\n");
        return;
    }

    if (fg)
        give_foreground(pid);

}

void
main_test_1_ipc(int argc, char ** argv)
{
    void * sem = create_sem(1, 0);
    if (sem == NULL) {
        printf("No se pudo crear el semaforo\n");
        return;
    }
    char * shm = shm_open(1);

    printf("Bloqueado\n");
    sem_down(sem);
    printf("Desbloqueado, mensaje leido: %s\n", shm);
}

void
main_test_2_ipc(int argc, char ** argv)
{
    void * sem = sem_get(1);
    char * shm = shm_get(1);

    scanf("%s\n", shm);

    sem_up(sem);
}


void
main_test_3_ipc(int argc, char ** argv)
{
    void * sem = sem_get(1);
    char * shm = shm_get(1);

    printf("Bloqueado\n");
    sem_down(sem);
    printf("Desbloqueado, mensaje leido: %s\n", shm);
}

void
main_ps(int argc, char ** argv)
{
    ps_list * list = (ps_list *) _sys_call(SYS_PS, 0, 0, 0);

    printf("\nPID  PPID  FG  STATUS\n");

    int i;
    char * fg_str;
    char * status_str;
    for (i = 0; i < list->nprocess; i++) {
        fg_str = (list->list[i].foreground == 0 ? "-" : "+");
        switch (list->list[i].status) {
            case 0:
                status_str = "P";
                break;

            case 1:
                status_str = "S";
                break;

            case 2:
                status_str = "R";
                break;
        }

        printf(" %d     %d    %s     %s\n", list->list[i].pid, list->list[i].parent_pid, fg_str, status_str);
    }
}

void
main_ipcs(int argc, char ** argv)
{
    shm_list * list = (shm_list *) _sys_call(SYS_IPCS, 0, 0, 0);

    int i;
    for (i = 0; i < list->nshm; i++) {
        printf("key: %d add: %x\n", list->addresses[i]->key, list->addresses[i]->start);
    }
}
