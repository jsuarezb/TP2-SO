#ifndef _VIDEO_H_
#define _VIDEO_H_ 

// _v → video driver

void _vClear();

void _vWrite(char c);

void _vWriteError(char c);

void _vWriteFormat(char c, char format);

void _vPrint(char* str);

void _vPrintError(char* str);

void _vDelete();

void _vMoveUp();

void _vNewLine();

void _vBackupScreen();

void _vRestoreScreen();

#endif
