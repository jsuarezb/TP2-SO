#ifndef _HANDLER_C_
#define _HANDLER_C_

void timertickHandler();
void keyboardHandler(unsigned char c);
void syscallHandler(uint64_t code, uint64_t arg1, uint64_t arg2, uint64_t arg3);

#endif
