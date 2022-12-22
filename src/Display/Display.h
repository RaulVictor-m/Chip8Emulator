#pragma once
#include <iostream>


class Display
{
public:

	static const uint32_t SCREEN_WIDTH = 64;
	static const uint32_t SCREEN_HIGHT = 32;
	uint8_t	display[SCREEN_WIDTH * SCREEN_HIGHT] = {};

	void cls();
	bool loadSprite(uint8_t* sprite, int size, int x, int y);

};

