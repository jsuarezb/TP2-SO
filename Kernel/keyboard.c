#include <stdint.h>
#include "include/define.h"
#include "include/keyboard.h"
#include "include/video.h"

extern struct KBD keyboard;

unsigned char keysTable[2][85] = {
	{
		0, ASCII_ESCAPE, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
		'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
		ASCII_CTRL, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', ASCII_SHIFT,
		'\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', ASCII_SHIFT, ASCII_PRTSC,
		ASCII_ALT, ' ', ASCII_CAPS, ASCII_F1, ASCII_F2, ASCII_F3, ASCII_F4, ASCII_F5, ASCII_F6,
		ASCII_F7, ASCII_F8, ASCII_F9, ASCII_F10, ASCII_NUMLOCK, ASCII_SCROLLLOCK, '7', '8', '9',
		'-', '4', '5', '6', '+', '1', '2', '3', '0', ASCII_DELETE, SYS_REQ
	},
	{
		0, ASCII_ESCAPE, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
		'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
		ASCII_CTRL, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', ASCII_SHIFT,
		'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', ASCII_SHIFT, ASCII_PRTSC,
		ASCII_ALT, ' ', ASCII_CAPS, ASCII_F1, ASCII_F2, ASCII_F3, ASCII_F4, ASCII_F5, ASCII_F6,
		ASCII_F7, ASCII_F8, ASCII_F9, ASCII_F10, ASCII_NUMLOCK, ASCII_SCROLLLOCK, '7', '8', '9',
		'-', '4', '5', '6', '+', '1', '2', '3', '0', ASCII_DELETE, SYS_REQ
	}
};

void KBDinitialize()
{
	int i;
	for (i = 0; i < KBD_BUFFER_SIZE; i++)
		keyboard.buffer[i] = 0;

	keyboard.writeIndex = 0;
	keyboard.readIndex = 0;
	keyboard.capsEnabled = FALSE;
	keyboard.shiftEnabled = FALSE;
	keyboard.ctrlEnabled = FALSE;
	keyboard.altEnabled = FALSE;
}

/*
 * Sets the corresponding key properties
 */
void setKey(unsigned char code)
{
	struct KBDKey key;
	int state = LOWERCASE;

	key.shown = FALSE;

	switch (code) {
		case LSHIFT_PRESSED:
		case RSHIFT_PRESSED:
			keyboard.shiftEnabled = TRUE;
			break;
		case LSHIFT_RELEASED:
		case RSHIFT_RELEASED:
			keyboard.shiftEnabled = FALSE;
			break;
		case CAPS_PRESSED:
			keyboard.capsEnabled = !keyboard.capsEnabled;
			break;
		case CAPS_RELEASED:
			break;
		case CTRL_PRESSED:
			keyboard.ctrlEnabled = TRUE;
			break;
		case CTRL_RELEASED:
			keyboard.ctrlEnabled = FALSE;
			break;
		default:
			if (!(code & 0x80)) {
				key.shown = TRUE;
				if ((isLetter(code) && keyboard.capsEnabled && !keyboard.shiftEnabled)
						|| (!keyboard.capsEnabled && keyboard.shiftEnabled)
						|| (!isLetter(code) && keyboard.capsEnabled && keyboard.shiftEnabled)) {
					state = UPPERCASE;
				}
			}

			break;
	}


	key.keyCode = code;
	key.asciiCode = keysTable[state][code];

	if (key.shown)
		addKey(key.asciiCode);
}

void addKey(unsigned char c)
{
	keyboard.buffer[keyboard.writeIndex++] = c;

	if (keyboard.writeIndex == KBD_BUFFER_SIZE)
		keyboard.writeIndex = 0;
}

unsigned char getKey()
{
	unsigned char c = keyboard.buffer[keyboard.readIndex];
	keyboard.buffer[keyboard.readIndex++] = 0;

	if (keyboard.readIndex == KBD_BUFFER_SIZE)
		keyboard.readIndex = 0;

	return c;
}

// Returns if the code of the pressed key is a letter
int isLetter(unsigned char c)
{
	if ((c >= 16 && c <= 25) || (c >= 30 && c <= 38) || (c >= 44 && c <= 50))
		return TRUE;

	return FALSE;
}

int isBufferEmpty()
{
	if (keyboard.buffer[keyboard.readIndex] != 0)
		return FALSE;

	return TRUE;
}
