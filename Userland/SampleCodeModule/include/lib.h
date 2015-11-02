#ifndef _LIB_H
#define _LIB_H

#include <stdint.h>

void _sys_call( uint64_t code, uint64_t arg1, uint64_t arg2, uint64_t arg3 );

/*
 * Writes a single character on screen
 */
void putChar(char c);

/*
 * Gets a character from keyboard buffer
 */
char getChar();

/*
 * Prints a formatted string
 */
void printf(char * fmt, ...);

/*
 * Scans from std input specified fmt
 */
int scanf(const char * fmt, ...);

/*
 * Stores the extracted values from `src` formatted as `fmt`
 */
int sscanf(const char * src, const char * fmt, ...);

/*
 * Stores int `i` representation on base `base` into `placeholder`
 */
void itos(int i, int base, char * placeholder);

/*
 * Concats two strings and writes the concatenation on `to`
 */
void concat(char * first, char * second, char * to);

/*
 * Compares two string until first difference
 * 0 if both are equal
 * not 0 if they differ
 */
int strcmp(const char * str1, const char * str2);

/*
 * Seeks for a char inside `str`
 * If found, returns the index of `c` inside `str`
 * If not found, returns -1
 */
int cindex(char c, const char * str);

void printf_v ( char* s, ... );
int int_length ( int i );
int strlen ( char* s );
void to_c (int i, char* to );
void to_hex( int i, char* to);
int stoi(const char * str);

#endif
