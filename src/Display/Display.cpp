#include "Display.h"

#define PIXELCHAR 1
#define CLSPIXEL  0

void Display::cls()
{

	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HIGHT; i++)
		display[i] = CLSPIXEL;
	//draw();
}


bool Display::loadSprite(uint8_t* sprite, int size, int x, int y)
{
	bool r = 0;
	if (x > 63) x %= 64;
	if (y > 31) y %= 32;

	for (int i = 0; i < size; i++)
	{
		int cursor = ((i + y) * SCREEN_WIDTH) + x;
		if (y + i > 31) break;
		for (size_t x2 = 0; x2 < 8; x2++)
		{
			if (x + x2 > 63) break;
			if (sprite[i] & (0b10000000 >> x2))
				if (display[cursor + x2] == PIXELCHAR) { display[cursor + x2] = CLSPIXEL; r = 1; }
				else display[cursor + x2] = PIXELCHAR;
		}


	}

	return r;
}