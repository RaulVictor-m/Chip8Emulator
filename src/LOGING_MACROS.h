#pragma once

//MACRO TO HELP BETTER NAVIGATE THROUGHT BUGS IN THE 'Decode()' FUNCTION

#if CHIP8_DEBUG == 1

	// OPENING THE DEBUG FILE
	static FILE* file;
#define OPEN_LOG file = fopen("LOG_DEBUG.txt", "w")

#define LOG_FILE(x,...) fprintf(file, x,__VA_ARGS__)

#define CLOSE_LOG fclose(file)


	// WRITING THE LOG FOR DECODE
#define NNN (code & 0x0fff)
#define KK	(code & 0x00ff)
#define VX	(reg_V[(code & 0x0f00)])
#define X	(code & 0x0f00)
#define VY	(reg_V[(code & 0x00f0)])
#define Y	(code & 0x00f0)
#define I	reg_I

#define NEXT_OPCODE ((MEMORY[reg_PC + 2] >> 8) + MEMORY[reg_PC + 3])

#define FLAG (reg_V[0xf])

#else 
	// TURNING OFF THE MACROS
#define OPEN_LOG
#define LOG_FILE(x,...)
#define CLOSE_LOG
#define NNN
#define KK
#define VX
#define VY
#define I
#define NEXT_OPCODE
#define FLAG

#endif 
