#ifndef _SCREENSAVER_H_
#define _SCREENSAVER_H

#define TICKS_TO_SECONDS		18
#define TICKS_PER_FRAME 		5
#define SCREENSAVER_WAIT_TIME	1 * TICKS_TO_SECONDS

void initScreensaver();

void startScreensaver();

void stopScreensaver();

void tickScreensaver();

void setScreensaverTime(int seconds);

void nextFrame();

void draw();

void drawFrame0();

void drawFrame1();

void drawFrame2();

#endif
