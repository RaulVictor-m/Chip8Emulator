#include "Display.h"

#define PIXELCHAR 1
#define CLSPIXEL  0

const uint8_t& Display::operator[](const int index) const
{
	return m_display[index];
}

void Display::cls()
{

	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
		m_display[i] = CLSPIXEL;

}


bool Display::loadSprite(const uint8_t* sprite, const int size, int x,  int y)
{
	bool r = 0;
	x %= 64;
	y %= 32;


	for (int i = 0; i < size; i++)
	{
		int cursor = ((i + y) * SCREEN_WIDTH) + x;

		if (y + i > 31) break;
		for (size_t sprite_x = 0; sprite_x < 8; sprite_x++)
		{
			if (x + sprite_x > 63) break;

			if (sprite[i] & (0b10000000 >> sprite_x))

				if (m_display[cursor + sprite_x] == PIXELCHAR) { m_display[cursor + sprite_x] = CLSPIXEL; r = 1; }

				else m_display[cursor + sprite_x] = PIXELCHAR;
		}


	}

	return r;
}