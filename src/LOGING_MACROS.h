#pragma once

//MACRO TO HELP BETTER NAVIGATE THROUGHT BUGS IN THE 'Decode()' FUNCTION
#ifdef DEBUG
#define CHIP8_DEBUG 0
#endif 

#if CHIP8_DEBUG == 1

    static FILE* file;
#define OPEN_LOG file = fopen("LOG_DEBUG.txt", "w")

#define LOG_FILE(x,...) fprintf(file, x,__VA_ARGS__)

#define CLOSE_LOG fclose(file)

    // THIS MACROS ARE ONLY USED TO MAKE THE LOGGING EASIER TO UNDERSTAND AND FIX AND ALSO BE ABLE TO SWITCH IN BETWEEN USING IT OR NOT
#define NNN (code & 0x0fff)
#define KK  (code & 0x00ff)
#define _X  ((code & 0x0f00)>>8)
#define _Y  ((code & 0x00f0)>>4)
#define VX  (reg_V[_X])
#define VY  (reg_V[_Y])
#define V0  reg_V[0]
#define I   reg_I

#define NEXT_OPCODE ((MEMORY[reg_PC + 2] << 8) + MEMORY[reg_PC + 3])

#define FLAG (reg_V[0xf])

#else 
    // TURNING OFF THE MACROS
#define OPEN_LOG
#define LOG_FILE(x,...)
#define CLOSE_LOG

#endif 
