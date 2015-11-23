#ifndef LIB_H
#define LIB_H

#include <stdint.h>
#include "paging.h"

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

void _cli(void);
void _sti(void);
void hlt(void);
uint64_t _asm_get_eflags(void);

void _reschedule(void);

int SetInts(int);

void _irq00handler();
void _irq01handler();
void _int80handler();
void _exc14handler();
void picMasterMask(uint16_t);
void picSlaveMask(uint16_t);

char *cpuVendor(char *result);

/**
 * Returns the cr3 value (64 bits)
 */
uint64_t _asm_get_cr3(void);

/**
 * Write the cr3 value into the processor
 */
void _asm_set_cr3(uint64_t cr3);

/**
 * Returns the EFLAGS
 */
uint64_t _asm_get_eflags(void);

#endif
