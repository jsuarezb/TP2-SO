#ifndef LIB_H
#define LIB_H

#include <stdint.h>


void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

void _cli(void);
void _sti(void);

void _irq00handler();
void _irq01handler();
void _int80handler();
void picMasterMask(uint16_t);
void picSlaveMask(uint16_t);

char *cpuVendor(char *result);

#endif
