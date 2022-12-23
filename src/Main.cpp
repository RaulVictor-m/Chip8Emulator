﻿#include "Chip8.h"


int main()
{
	Chip8 emulator;
	
	if (emulator.InitEmulator("IMPORTED-ROMS/TETRIS") && emulator.Construct(64 * 1, 32 * 1, 12, 12))
		emulator.Start();

	return 0;
}