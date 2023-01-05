#pragma once
#include "Chip8.h"
#include "Display/Display.h"


 

inline void Chip8::OP_00E0(const uint16_t code)		     //	=	CLS
{
	display.cls();
	
	//time
	m_instruction_time = 109;
}
inline void Chip8::OP_00EE(const uint16_t code)		     //	=	RET
{
	reg_SP -= 2;
	reg_PC = *((uint16_t*)(&MEMORY[reg_SP]));

	//time
	m_instruction_time = 100;
}
inline void Chip8::OP_0NNN(const uint16_t code)          //	=	JP, nnn (LEGACY-SUPORT)	
{

}
inline void Chip8::OP_1NNN(const uint16_t code)          //	=	JP, nnn
{
	reg_PC = (code ^ 0x1000);
	reg_PC -= 2;

	//time
	m_instruction_time = 100;
}
inline void Chip8::OP_2NNN(const uint16_t code)          //	=	Call, nnn
{
	*((uint16_t*)(&MEMORY[reg_SP])) = reg_PC;
	reg_SP += 2;
	reg_PC = (code ^ 0x2000);
	reg_PC -= 2;

	//time
	m_instruction_time = 100;
}
inline void Chip8::OP_3XKK(const uint16_t code)          //	=	IF(VX == KK) skip next instruction
{
	const int x = ((code & 0x0F00) >> 8);
	if (reg_V[x] == (0x00FF & code))
		reg_PC += 2;

	//time
	m_instruction_time = 50;
}
inline void Chip8::OP_4XKK(const uint16_t code)          //	=	IF!(VX == KK) skip next instruction
{
	const int x = ((code & 0x0F00) >> 8);
	if (reg_V[x] != (0x00FF & code))
		reg_PC += 2;

	//time
	m_instruction_time = 50;
}
inline void Chip8::OP_5XY0(const uint16_t code)          //	=	IF(VX == VY) skip next instruction
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	if (reg_V[x] == reg_V[y])
		reg_PC += 2;

	//time
	m_instruction_time = 70;
}
inline void Chip8::OP_6XKK(const uint16_t code)          //	=	MOV VX, KK
{
	const int x = ((code & 0x0F00) >> 8);
	reg_V[x] = (0x00FF & code);

	//time
	m_instruction_time = 27;
}
inline void Chip8::OP_7XKK(const uint16_t code)          //	=	ADD VX, KK
{
	const int x = ((code & 0x0F00) >> 8);
	reg_V[x] += (0x00FF & code);

	//time
	m_instruction_time = 45;
}
inline void Chip8::OP_8XY0(const uint16_t code)          //	=	MOV VX, VY
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	reg_V[x] = reg_V[y];

	//time
	m_instruction_time = 200;
}
inline void Chip8::OP_8XY1(const uint16_t code)          //	=	VX = VX or VY
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	reg_V[x] = (reg_V[x] | reg_V[y]);

	//time
	m_instruction_time = 200;
}
inline void Chip8::OP_8XY2(const uint16_t code)          //	=	VX = VX and VY
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	reg_V[x] = (reg_V[x] & reg_V[y]);

	//time
	m_instruction_time = 200;
}
inline void Chip8::OP_8XY3(const uint16_t code)          //	=	VX = VX xor VY
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	reg_V[x] = (reg_V[x] ^ reg_V[y]);

	//time
	m_instruction_time = 200;
}
inline void Chip8::OP_8XY4(const uint16_t code)          //	=	VX = VX + VY	: SET FLAG
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	const int result = (reg_V[x] + reg_V[y]);
	reg_V[0xF] = (result > 0xFF);
	reg_V[x] = (uint8_t)result;

	//time
	m_instruction_time = 200;
}
inline void Chip8::OP_8XY5(const uint16_t code)          //	=	VX = VX - VY	: SET FLAG
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	reg_V[0xf] = (reg_V[x] > reg_V[y]);
	reg_V[x] = (reg_V[x] - reg_V[y]);

	//time
	m_instruction_time = 200;
}
inline void Chip8::OP_8XY6(const uint16_t code)          //	=	VX = VX SHR 1	: SET FLAG
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	//reg_V[x] = reg_V[y];
	reg_V[0xF] = (reg_V[x] & 0b00000001);
	reg_V[x] = (reg_V[x] >> 1);

	//time
	m_instruction_time = 200;
}
inline void Chip8::OP_8XY7(const uint16_t code)          //	=	VX = VY - VX	: SET FLAG
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	reg_V[0xf] = (reg_V[x] < reg_V[y]);
	reg_V[x] = (reg_V[y] - reg_V[x]);

	//time
	m_instruction_time = 200;
}
inline void Chip8::OP_8XYE(const uint16_t code)          //	=	VX = VX SHL 1	: SET FLAG
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	//reg_V[x] = reg_V[y];
	reg_V[0xf] = ((reg_V[x] & 0b10000000) >> 7);
	reg_V[x] = (reg_V[x] << 1);

	//time
	m_instruction_time = 200;
}
inline void Chip8::OP_9XY0(const uint16_t code)          //	=	IF!(VX == VY) skip next instruction
{
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	if (reg_V[x] != reg_V[y])
		reg_PC += 2;

	//time
	m_instruction_time = 70;
}
inline void Chip8::OP_ANNN(const uint16_t code)          //	=	MOV I, nnn
{
	reg_I = (code & 0x0FFF);

	//time
	m_instruction_time = 55;
}
inline void Chip8::OP_BNNN(const uint16_t code)          //	=	JP, V0+nnn
{
	reg_PC = (code & 0x0FFF) + (reg_V[0] - 2);

	//time
	m_instruction_time = 105;
}
inline void Chip8::OP_CXKK(const uint16_t code)          //	=	MOV VX, (rand & KK)
{
	const int random = (CHIP8_MICROSEC_NOW % (0xFF+1)) & (code & 0x00FF);
	const int x = ((code & 0x0F00) >> 8);
	reg_V[x] = (uint8_t)random;

	//time
	m_instruction_time = 164;
}
inline void Chip8::OP_DXYN(const uint16_t code)          //	=	DRW Vx, Vy, nibble  :	SET FLAG
{
	const int N = (0x000F & code);
	const int x = ((code & 0x0F00) >> 8);
	const int y = ((code & 0x00F0) >> 4);
	reg_V[0xF] = loadSprite(N, x, y);
	DrawTheDisplay();

	//time
	m_instruction_time = 18000;//22734;
}
inline void Chip8::OP_EX9E(const uint16_t code)          //	=	IF(KEY[VX] == PRESSED) skip next instruction
{
	const int x = ((code & 0x0F00) >> 8);
	reg_PC += 2 * (keyIsPressed(reg_V[x]));

	//time
	m_instruction_time = 73;
}
inline void Chip8::OP_EXA1(const uint16_t code)          //	=	IF(KEY[VX] != PRESSED) skip next instruction
{
	const int x = ((code & 0x0F00) >> 8);
	reg_PC += 2 * (!(keyIsPressed(reg_V[x])));

	//time
	m_instruction_time = 73;
}
inline void Chip8::OP_FX07(const uint16_t code)          //	=	Vx = DT
{
	const int x = ((code & 0x0F00) >> 8);
	reg_V[x] = reg_DT;

	//time
	m_instruction_time = 45;
}
inline void Chip8::OP_FX0A(const uint16_t code)          //	=	WAIT keypress and store in Vx
{
	const int x = ((code & 0x0F00) >> 8);
	reg_V[x] = waitForKeyPress();

	//time
	m_instruction_time = 0;
}
inline void Chip8::OP_FX15(const uint16_t code)          //	=	DT = Vx 
{
	const int x = ((code & 0x0F00) >> 8);
	reg_DT = reg_V[x];

	//time
	m_instruction_time = 45;
}
inline void Chip8::OP_FX18(const uint16_t code)          //	=	ST = Vx 
{
	const int x = ((code & 0x0F00) >> 8);
	reg_ST = reg_V[x];

	//time
	m_instruction_time = 45;
}
inline void Chip8::OP_FX1E(const uint16_t code)          //	=	SET I = I + Vx
{
	const int x = ((code & 0x0F00) >> 8);
	reg_I += reg_V[x];

	//time
	m_instruction_time = 86;
}
inline void Chip8::OP_FX29(const uint16_t code)          //	=	SET I = NUMBER-VX(sprite)
{
	const int x = ((code & 0x0F00) >> 8);
	reg_I = 0x100 + (reg_V[x] * 5);

	//time
	m_instruction_time = 91;
}
inline void Chip8::OP_FX33(const uint16_t code)          //	=   SET I = VX Decimal Numbers
{
	const int x = ((code & 0x0F00) >> 8);
	MEMORY[reg_I] = reg_V[x] / 100;
	MEMORY[reg_I + 1] = (reg_V[x] / 10) % 10;
	MEMORY[reg_I + 2] = reg_V[x] % 10;

	//time
	m_instruction_time = 927;
}
inline void Chip8::OP_FX55(const uint16_t code)          //	=	PUSH at I
{
	const int x = ((code & 0x0F00) >> 8);
	for (int i = 0; i <= x; i++)
	{
		MEMORY[reg_I + i] = reg_V[i];
	}
	reg_I += x + 1;

	//time
	m_instruction_time = 605;
}
inline void Chip8::OP_FX65(const uint16_t code)          //	=	POP at I
{
	const int x = ((code & 0x0F00) >> 8);
	for (int i = 0; i <= x; i++)
	{
		reg_V[i] = MEMORY[reg_I + i];
	}
	reg_I += x + 1;

	//time
	m_instruction_time = 605;
}






