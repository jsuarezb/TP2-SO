#include <stdint.h>
#include "include/define.h"
#include "include/video.h"
#include "include/screensaver.h"
#include "include/life.h"

extern char cells[VIDEO_HEIGHT][VIDEO_WIDTH];

static void (*frameDrawers[FRAMES])(void);
static int currentFrame = 0;
static char counter = 0;

unsigned int tickCount = TICKS_PER_FRAME;
unsigned int showingScreensaver = FALSE;
unsigned int timerLimit = 20 * TICKS_TO_SECONDS;
unsigned int timer = 20 * TICKS_TO_SECONDS;

/*
 * Draw the first frame of the screensaver
 */
void initScreensaver()
{
	frameDrawers[0] = &drawFrame0;
	frameDrawers[1] = &drawFrame1;
	frameDrawers[2] = &drawFrame2;
	currentFrame = 0;
	counter = 0;

	_vClear();
	setupLife();
	draw();
}

/*
 * Saves the current screen and shows the screensaver
 */
void startScreensaver()
{
	showingScreensaver = TRUE;
	_vBackupScreen();
	initScreensaver();
}

/*
 * Stops the screensaver and resumes activity
 */
void stopScreensaver()
{
	_vRestoreScreen();
	showingScreensaver = FALSE;
}

/*
 * Frame manager for the screensaver
 */
void tickScreensaver()
{
	if (tickCount == 0) {
		nextFrame();
		tickCount = TICKS_PER_FRAME;
	} else {
		tickCount--;
	}
}

/*
 * Sets a new wait time
 */
void setScreensaverTime(int seconds)
{
	timerLimit = seconds * TICKS_TO_SECONDS;
	timer = (timer > timerLimit) ? timerLimit : timer;
}

/*
 * Go to the next frame and draw it
 */
void nextFrame()
{
	currentFrame = (currentFrame + 1) % FRAMES;
	draw();
}

/*
 * Fill the screen with the corresponding frame
 */
void draw()
{
	// (*frameDrawers[currentFrame])();
	int x, y;

	nextGen();
	for (y = 0; y < VIDEO_HEIGHT; y++) {
		for (x = 0; x < VIDEO_WIDTH; x++) {
			if (cells[y][x] == 1)
				_vWriteFormat(' ', RED_BG | RED_FG);
			else
				_vWriteFormat(' ', BLACK_BG | WHITE_FG);
		}
	}
}

// In case I want to draw every frame by hand
// it can be done using an array
// of functions that draw the corresponding frame; another
// way is to define an array containing each screen for
// every frame. This'd be hard to do by hand, so
// I'm sticking with a simple drawing function

void drawFrame0()
{
	int x, y;
	_vClear();

	for (y = 0; y < VIDEO_HEIGHT; y++) {
		for (x = 0; x < VIDEO_WIDTH; x++) {
			if (x == 4 && y == 6)
				_vWriteFormat(' ', WHITE_BG | WHITE_FG);
			else
				_vWriteFormat(' ', BLACK_BG | WHITE_FG);
		}
	}
}

void drawFrame1()
{
	int x, y;
	_vClear();

	for (y = 0; y < VIDEO_HEIGHT; y++) {
		for (x = 0; x < VIDEO_WIDTH; x++) {
			_vWriteFormat(' ', BLACK_BG | WHITE_FG);
		}
	}
}

void drawFrame2()
{
	int x, y;
	_vClear();

	for (y = 0; y < VIDEO_HEIGHT; y++) {
		for (x = 0; x < VIDEO_WIDTH; x++) {
			if (x == 4 && y == 6)
				_vWriteFormat(' ', BLACK_BG | WHITE_FG);
			else
				_vWriteFormat(' ', WHITE_BG | WHITE_FG);
		}
	}
}
