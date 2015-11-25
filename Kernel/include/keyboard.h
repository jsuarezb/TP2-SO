#ifndef _KEYBOARD_H_
#define _KEYBOARD_H

void setKey(unsigned char code);
void KBDinitialize();
void addKey(unsigned char c);
unsigned char getKey();
int isLetter(unsigned char c);
int isBufferEmpty();
int isBufferFull();

#endif
