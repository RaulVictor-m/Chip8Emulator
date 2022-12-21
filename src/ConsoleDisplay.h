#pragma once
#include <iostream>
#include <windows.h>

class ConsoleDisplay
{
private:
	static const uint32_t SCREEN_WIDTH = 64;
	static const uint32_t SCREEN_HIGHT = 32;
	uint8_t	display[SCREEN_WIDTH * SCREEN_HIGHT] = {};


	UCHAR consoleScreen[120 * 40] = {};
	HANDLE hConsole;

public:
	ConsoleDisplay();
	void cls();
	bool loadSprite(uint8_t* sprite, int size, int x, int y);
	void draw();

	void PrintParans(const char* str, uint8_t* mem, uint8_t pc);

};

