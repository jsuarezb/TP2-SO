/*
 * Video driver
 */

#include "include/define.h"
#include "include/video.h"
#define VIDEO_SIZE width * height

static char * const video = (char*) VIDEO_START;
static int vOffset = 0;
static const int width = VIDEO_WIDTH;
static const int height = VIDEO_HEIGHT;

struct ScreenBackup screenBackup;

/*
 * Clears the screen
 */
void _vClear() {
	int x;

	vOffset = 0;
	for (x = 0; x < VIDEO_SIZE; x++)
		_vWrite(' ');

	vOffset = 0;
}

/*
 * Writes `c` on screen
 */
void _vWrite(char c) {
	_vWriteFormat(c, DEFAULT_CONSOLE_FORMAT);
}

/*
 * Writes `c` on screen as an error
 */
void _vWriteError(char c) {
	_vWriteFormat(c, ERROR_CONSOLE_FORMAT);
}

/*
 * Writes `c` on screen with `format` format
 */
void _vWriteFormat(char c, char format) {
	// Filter special characters
	switch (c) {
		case '\n':
			_vNewLine();
			break;
		case '\b':
			_vDelete();
			break;
		default:
			video[vOffset] = c;
			video[vOffset + 1] = format;
			vOffset += 2;
			break;
	}

	if (vOffset >= 2 * VIDEO_SIZE)
		_vMoveUp();

}

/*
 * Writes on screen the string defined by `c`
 */
void _vPrint(char* c) {
	while(*c) _vWrite(*c++);
}

/*
 * Writes on screen the string defined by `c` as an error
 */
void _vPrintError(char* c) {
	while(*c) _vWriteError(*c++);
}

/*
 * Deletes a character from the screen and moves back the cursor
 */
void _vDelete() {
	if (vOffset > 0) {
		vOffset -= 2;
		video[vOffset] = ' ';
		video[vOffset + 1] = DEFAULT_CONSOLE_FORMAT;
	}
}

/*
 * Moves the screen up by one line
 */
void _vMoveUp() {
	int i;

	for (i = 0; i < 2 * (VIDEO_SIZE - width); i++)
		video[i] = video[i + 2 * width];

	for (; i < 2 * VIDEO_SIZE; i++)
		video[i] = i % 2 == 0 ? ' ' : DEFAULT_CONSOLE_FORMAT;

	vOffset -= 2 * width;
}

/*
 * Fills the characters between the cursor and the end of the line
 * with spaces and leaves the cursor at the start of the next line
 */
void _vNewLine() {
	// Fill line with spaces till the end
	do {
		_vWrite(' ');
	} while ((vOffset / 2) % width > 0);

}

/*
 * Saves the screen in a backup array
 */
void _vBackupScreen() {
	int i = 0;

	screenBackup.cursor = vOffset;
	while (i < VIDEO_SIZE * 2) {
		screenBackup.video[i] = video[i];
		i++;
	}
}

/*
 * Restores the screen saved in the backup array
 */
void _vRestoreScreen() {
	int i = 0;

	vOffset = screenBackup.cursor;
	while (i < VIDEO_SIZE * 2) {
		video[i] = screenBackup.video[i];
		i++;
	}
}
