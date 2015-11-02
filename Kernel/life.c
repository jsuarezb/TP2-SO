#include "include/define.h"
#include "include/life.h"

char cells[VIDEO_HEIGHT][VIDEO_WIDTH];
char nextCells[VIDEO_HEIGHT][VIDEO_WIDTH];

void setupLife()
{
	int i, j;

	for (i = 0; i < VIDEO_HEIGHT; i++)
		for (j = 0; j < VIDEO_WIDTH; j++)
			cells[i][j] = 0;

	// 10 cell row
	int y = VIDEO_HEIGHT / 2, x = VIDEO_WIDTH / 2 - 5;
	for (i = 0; i <= 10; i++)
		cells[y][x + i] = 1;

}

void nextGen()
{
	int x, y, neighbors;

	for (y = 0; y < VIDEO_HEIGHT; y++) {
		for (x = 0; x < VIDEO_WIDTH; x++) {
			neighbors = countNeighbors(x, y);

			nextCells[y][x] = 0;
			if (cells[y][x] == 0 && neighbors == 3)
				nextCells[y][x] = 1;
			else if (neighbors == 2 || neighbors == 3)
				nextCells[y][x] = 1;
		}
	}

	updateCells();
}

int countNeighbors(int x, int y) {
	int i, j, n = 0;

	for (i = y - 1; i <= y + 1; i++) {
		if (i < 0 || i >= VIDEO_HEIGHT)
			continue;

		for (j = x - 1; j <= x + 1; j++) {
			if (j < 0 || j >= VIDEO_WIDTH)
				continue;

			if (x == j && y == i)
				continue;

			if (cells[i][j] == 1)
				n++;
		}
	}

	return n;
}

void updateCells()
{
	int x, y;

	for (y = 0; y < VIDEO_HEIGHT; y++) {
		for (x = 0; x < VIDEO_WIDTH; x++) {
			cells[y][x] = nextCells[y][x];
		}
	}
}
