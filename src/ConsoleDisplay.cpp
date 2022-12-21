#include "ConsoleDisplay.h"

#define PIXELCHAR ' '
#define CLSPIXEL  219

ConsoleDisplay::ConsoleDisplay()
{
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);

}

void ConsoleDisplay::cls()
{

	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HIGHT; i++)
		display[i] = CLSPIXEL;
	//draw();
}


bool ConsoleDisplay::loadSprite(uint8_t* sprite, int size, int x, int y)
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
	draw();
	return r;
}

void ConsoleDisplay::draw()
{

	DWORD dwBytesWritten = 0;

	for (int y = 0; y < SCREEN_HIGHT; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			consoleScreen[(y * 120) + x+28] = display[(y * SCREEN_WIDTH) + x];

		}

	}
	WriteConsoleOutputCharacter(hConsole, (LPCSTR)consoleScreen, 120 * 32 , { 0,0 }, &dwBytesWritten);



}


void ConsoleDisplay::PrintParans(const char *str, uint8_t* mem, uint8_t pc)
{
	// hard coded loop just to get the functionality going
	for (size_t y = 0; y < 21; y++)
	{
		for (size_t x = 0; x < 32; x++)
		{
			if (str[y * 12 + x] == ';')break;
			consoleScreen[y * 120 + x+(64+29)] = str[y * 12 + x];

		}

	}

	

}
