#ifndef	_SHELL_H_
#define	_SHELL_H_

/*
 * Starts the shell
 */
void startShell();

/*
 * Prints shell line header
 */
void printLineStart();

/*
 *
 */
void parseCommand(const char * line);

#endif