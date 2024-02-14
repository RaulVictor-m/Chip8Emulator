#define OLC_PGE_APPLICATION
#include "Chip8.h"
#include "OPCODES.h"
#include "LOGING_MACROS.h"
#include "Display.h"
#include <chrono>
#include <iostream>

Chip8::Chip8()
{
    MEMORY[0x100] = 0xF0; MEMORY[0x101] = 0x90; MEMORY[0x102] = 0x90; MEMORY[0x103] = 0x90; MEMORY[0x104] = 0xF0;//0
    MEMORY[0x105] = 0x20; MEMORY[0x106] = 0x60; MEMORY[0x107] = 0x20; MEMORY[0x108] = 0x20; MEMORY[0x109] = 0x70;//1
    MEMORY[0x10a] = 0xF0; MEMORY[0x10b] = 0x10; MEMORY[0x10c] = 0xF0; MEMORY[0x10d] = 0x80; MEMORY[0x10e] = 0xF0;//2
    MEMORY[0x10f] = 0xF0; MEMORY[0x110] = 0x10; MEMORY[0x111] = 0xF0; MEMORY[0x112] = 0x10; MEMORY[0x113] = 0xF0;//3
    MEMORY[0x114] = 0x90; MEMORY[0x115] = 0x90; MEMORY[0x116] = 0xF0; MEMORY[0x117] = 0x10; MEMORY[0x118] = 0x10;//4
    MEMORY[0x119] = 0xF0; MEMORY[0x11a] = 0x80; MEMORY[0x11b] = 0xF0; MEMORY[0x11c] = 0x10; MEMORY[0x11d] = 0xF0;//5
    MEMORY[0x11e] = 0xF0; MEMORY[0x11f] = 0x80; MEMORY[0x120] = 0xF0; MEMORY[0x121] = 0x90; MEMORY[0x122] = 0xF0;//6
    MEMORY[0x123] = 0xF0; MEMORY[0x124] = 0x10; MEMORY[0x125] = 0x20; MEMORY[0x126] = 0x40; MEMORY[0x127] = 0x40;//7
    MEMORY[0x128] = 0xF0; MEMORY[0x129] = 0x90; MEMORY[0x12a] = 0xF0; MEMORY[0x12b] = 0x90; MEMORY[0x12c] = 0xF0;//8
    MEMORY[0x12d] = 0xF0; MEMORY[0x12e] = 0x90; MEMORY[0x12f] = 0xF0; MEMORY[0x130] = 0x10; MEMORY[0x131] = 0xF0;//9
    MEMORY[0x132] = 0xF0; MEMORY[0x133] = 0x90; MEMORY[0x134] = 0xF0; MEMORY[0x135] = 0x90; MEMORY[0x136] = 0x90;//A
    MEMORY[0x137] = 0xE0; MEMORY[0x138] = 0x90; MEMORY[0x139] = 0xE0; MEMORY[0x13a] = 0x90; MEMORY[0x13b] = 0xE0;//B
    MEMORY[0x13c] = 0xF0; MEMORY[0x13d] = 0x80; MEMORY[0x13e] = 0x80; MEMORY[0x13f] = 0x80; MEMORY[0x140] = 0xF0;//C
    MEMORY[0x141] = 0xE0; MEMORY[0x142] = 0x90; MEMORY[0x143] = 0x90; MEMORY[0x144] = 0x90; MEMORY[0x145] = 0xE0;//D
    MEMORY[0x146] = 0xF0; MEMORY[0x147] = 0x80; MEMORY[0x148] = 0xF0; MEMORY[0x149] = 0x80; MEMORY[0x14a] = 0xF0;//E
    MEMORY[0x14b] = 0xF0; MEMORY[0x14c] = 0x80; MEMORY[0x14d] = 0xF0; MEMORY[0x14e] = 0x80; MEMORY[0x14f] = 0x80;//F

    OPEN_LOG;
    sAppName = "Chip8 Emulator";
}

Chip8::~Chip8()
{
    CLOSE_LOG;
}

bool Chip8::OnUserCreate()
{
    
    return true;
}

bool Chip8::OnUserUpdate(float fElapsedTime) 
{
    //To do Implement a menu to select the ROM from the UI
    uint16_t code = MEMORY[reg_PC];
    code = (code << 8);
    code = code + MEMORY[reg_PC + 1];

    if ((CHIP8_MICROSEC_NOW - m_main_clock) > m_instruction_time)
    {
        m_main_clock = CHIP8_MICROSEC_NOW;
        Decode(code);
        reg_PC += 2;
    }

    if ((CHIP8_MICROSEC_NOW - m_timers_clock) > 16666)
    {
        m_timers_clock = CHIP8_MICROSEC_NOW;
        reg_DT = reg_DT - (reg_DT > 0);
        //TODO: handle sound properly
    }
    return true;
}

bool Chip8::InitEmulator(const char* fileName)
{
    if (!loadFile(fileName)) return 0;


    m_main_clock = CHIP8_MICROSEC_NOW;
    m_timers_clock = m_main_clock;

    reg_PC = 0x200;
    return 1;
}

bool Chip8::loadFile(const char* fileName)
{
    unsigned char* rom_start = &MEMORY[0x200];
    FILE* file = fopen(fileName, "rb");
    if (file == NULL)
    {
        std::cout << "error";
        return false;
    }
    fread(rom_start, 4096 - 0x200, 1, file);
    fclose(file);
    return true;
}

void Chip8::Decode(const uint16_t code)
{
    LOG_FILE("Address  =  %.4X      |        Opcode   =   %.4X         |         ", reg_PC, code);

    const int LOW_BYTE = (code & 0x00FF);

    switch (code & 0xf000)
    {

    case 0x0000:
        //code == 00E0  =   CLS
        if ((code & 0x00FF) == 0x00E0)
        {
            LOG_FILE("CLS \n");
            OP_00E0(code);
        }
        //code == 00EE  =   RET
        else if ((code & 0x00FF) == 0x00EE)
        {
            LOG_FILE("RET \n");
            OP_00EE(code);
        }
        break;

        //code == 1NNN  =   JP, nnn
    case 0x1000:
        LOG_FILE("JP, %.4X\n", NNN);
        OP_1NNN(code);
        break;

        //code == 2NNN  =   Call, nnn
    case 0x2000:
        LOG_FILE("CALL, %.4X\n", NNN);
        OP_2NNN(code);
        break;

        //code == 3XKK  =   IF(VX == KK) skip next instruction
    case 0x3000:
        LOG_FILE("SKIP NEXT(%.4X) IF V%d(%.4X) == %.4X\n", NEXT_OPCODE, _X, VX, KK);
        OP_3XKK(code);
        break;

        //code == 4XKK  =   IF!(VX == KK) skip next instruction
    case 0x4000:
        LOG_FILE("SKIP NEXT(%.4X) IF V%d(%.4X) != %.4X\n", NEXT_OPCODE, _X, VX, KK);
        OP_4XKK(code);
        break;


        //code == 5XY0  =   IF(VX == VY) skip next instruction
    case 0x5000:
        LOG_FILE("SKIP NEXT(%.4X) IF V%d(%.4X) != V%d(%.4X)\n", NEXT_OPCODE, _X, VX, _Y, VY);
        OP_5XY0(code);
        break;

        //code == 6XKK  =   MOV VX, KK
    case 0x6000:
        LOG_FILE("LD V%d, %.4X\n",_X, KK);
        OP_6XKK(code);
        break;

        //code == 7XKK  =   ADD VX, KK
    case 0x7000:
        LOG_FILE("ADD V%d(%.4X), %.4X\n",_X,VX,KK);
        OP_7XKK(code);
        break;

        //code == 8XY0 to 8XYF  =   OPERATORS
    case 0x8000:


        switch (code & 0x000F)
        {
        case 0:                         //code == 8XY0  =   MOV VX, VY
            LOG_FILE("LD V%d(%.4X), V%d(%.4X)\n", _X, VX, _Y, VY);
            OP_8XY0(code);
            break;
        case 1:                         //code == 8XY1  =   VX = VX or VY
            LOG_FILE("OR V%d(%.4X), V%d(%.4X)\n", _X, VX, _Y, VY);
            OP_8XY1(code);
            break;
        case 2:                         //code == 8XY2  =   VX = VX and VY
            LOG_FILE("AND V%d(%.4X), V%d(%.4X)\n", _X, VX, _Y, VY);
            OP_8XY2(code);
            break;
        case 3:                         //code == 8XY3  =   VX = VX xor VY
            LOG_FILE("XOR V%d(%.4X), V%d(%.4X)\n", _X, VX, _Y, VY);
            OP_8XY3(code);
            break;
        case 4:                         //code == 8XY4  =   VX = VX + VY    : SET FLAG
            LOG_FILE("ADD V%d(%.4X), V%d(%.4X)          ", _X, VX, _Y, VY);
            OP_8XY4(code);
            LOG_FILE("SETFLAG: %.4X\n", FLAG);
            break;
        case 5:                         //code == 8XY5  =   VX = VX - VY    : SET FLAG
            LOG_FILE("SUB V%d(%.4X), V%d(%.4X)          ", _X, VX, _Y, VY);
            OP_8XY5(code);
            LOG_FILE("SETFLAG: %.4X\n", FLAG);
            break;
        case 6:                         //code == 8XY6  =   VX = VX SHR 1   : SET FLAG
            LOG_FILE("SHR 1 V%d(%.4X)          ", _X, VX);
            OP_8XY6(code);
            LOG_FILE("SETFLAG: %.4X\n", FLAG);
            break;
        case 7:                         //code == 8XY7  =   VX = VY - VX    : SET FLAG
            LOG_FILE("SUB V%d(%.4X), V%d(%.4X)          ", _Y, VY, _X, VX);
            OP_8XY7(code);
            LOG_FILE("SETFLAG: %.4X\n", FLAG);
            break;
        case 0x000E:                    //code == 8XYE  =   VX = VX SHL 1   : SET FLAG
            LOG_FILE("SHL 1 V%d(%.4X)          ", _X, VX);
            OP_8XYE(code);
            LOG_FILE("SETFLAG: %.4X\n", FLAG);
            break;
        }

        break;

        //code == 9XY0  =   IF!(VX == VY) skip next instruction
    case 0x9000:
        LOG_FILE("SKIP NEXT(%.4X) IF (V%d = %.4X)  != (V%d = %.4X)\n",NEXT_OPCODE, _X, VX, _Y, VY);
        OP_9XY0(code);
        break;

        //code == ANNN  =   MOV I, nnn
    case 0xA000:
        LOG_FILE("LD I, %.4X\n", I);
        OP_ANNN(code);
        break;

        //code == BNNN  =   JP, V0+nnn
    case 0xB000:
        LOG_FILE("JP (V0 = %.4X) + %.4X\n", V0, reg_PC + 2);
        OP_BNNN(code);
        break;

        //code == CXKK  =   MOV VX, (rand & KK)
    case 0xC000:
        LOG_FILE("LD V%d, (random = ", _X);
        OP_CXKK(code);
        LOG_FILE("%.4X)\n", VX);
        break;

        //code == DXYN  =   DRW Vx, Vy, nibble  :   SET FLAG
    case 0xD000:
        LOG_FILE("Load Sprites to X - %d, Y - %d   ", _X, _Y);
        OP_DXYN(code);
        LOG_FILE("SETFLAG %d\n", FLAG);
        break;

        //code == EX9E  =   IF(KEY[VX] == PRESSED) skip next instruction
        //code == EXA1  =   IF(KEY[VX] != PRESSED) skip next instruction
    case 0xE000:


        if (LOW_BYTE == 0x9E)   //code == EX9E
        {
            LOG_FILE("SKIP NEXT(%.4X) IF KEY[V%d](%.4X)  == PRESSED\n", NEXT_OPCODE, _X, VX);
            OP_EX9E(code);
        }
        else if (LOW_BYTE == 0xA1)          //code == EXA1
        {
            LOG_FILE("SKIP NEXT(%.4X) IF KEY[V%d](%.4X)  != PRESSED\n", NEXT_OPCODE, _X, VX);
            OP_EXA1(code);
        }
        break;

        //code == FX00  -   FXFF   
    case 0xF000:

        switch (code & 0x00FF)
        {
        case 0x07:                          //code == FX07  =   Vx = DT
            LOG_FILE("LD V%d, DT(%.4X)\n", _X, reg_DT);
            OP_FX07(code);
            break;

        case 0x0A:                          //code == FX0A  =   WAIT keypress and store in Vx
            LOG_FILE("LD V%d, KEY - PRESSED\n", _X);
            OP_FX0A(code);
            break;

        case 0x15:                          //code == FX15  =   DT = Vx 
            LOG_FILE("LD DT, V%d(%.4X)\n", _X, VX);
            OP_FX15(code);
            break;

        case 0x18:                          //code == FX18  =   ST = Vx 
            LOG_FILE("LD ST, V%d(%.4X)\n", _X, VX);
            OP_FX18(code);
            break;

        case 0x1E:                          //code == FX1E  =   SET I = I + Vx
            LOG_FILE("SET I, I(%.4X) + V%d(%.4X)\n", I, _X, VX);
            OP_FX1E(code);
            break;

        case 0x29:                          //code == FX29  =   SET I = NUMBER-VX(sprite)
            LOG_FILE("LD I, NUMBER-V%d(%.4X)\n",_X,VX);
            OP_FX29(code);
            break;

        case 0x33:                          //code == FX33  =   SET I = VX Decimal Numbers Representation
            LOG_FILE("SET [I,I+2,i+3], Decimal Numbers Representation of V%d(%.4X)\n", _X,VX);
            OP_FX33(code);
            break;

        case 0x55:                          //code == FX55  =   PUSH at I
            LOG_FILE("PUSH V0 - V%d at I(%.4X)\n",_X, I);
            OP_FX55(code);
            break;

        case 0x65:                          //code == FX65  =   POP at I
            LOG_FILE("POP V0 - V%d at I(%.4X)\n", _X, I);
            OP_FX65(code);
            break;

        }
        break;
    default:
        LOG_FILE("UNUSED\n");
    

    }
    LOG_FILE("\n");

}

inline int Chip8::loadSprite(const int size,const int x,const int y)
{
    return display.loadSprite(&MEMORY[reg_I], size,reg_V[x],reg_V[y]);
}

void Chip8::DrawTheDisplay()
{
    Clear(olc::WHITE);
    for (int x = 0; x < display.SCREEN_WIDTH; x++)
        for (int y = 0; y < display.SCREEN_HEIGHT; y++)
        {
            if (display[(y * display.SCREEN_WIDTH) + x])
                Draw(x, y, olc::BLACK);
        }
}

bool Chip8::keyIsPressed(const int key_code)
{
    // int keys[16] = {olc::A};

    int key = (key_code > 9) ? key_code - ((olc::A * 0xA) - 1) : key_code + olc::K0;

    return GetKey((olc::Key)key).bHeld;
}

uint8_t Chip8::waitForKeyPress()
{
    while(true)
    { 
        for (int i = 0; i <= 0xf; i++)
        {
            if (keyIsPressed(i)) return (uint8_t)i;
        }
    }
    return 0x0;
}

inline static void toHex(char* output, int value, int value_size)
{
    int temp = 0;
    int i = value_size - 1;
    while (value != 0) // taking hex from the register value into buffer
    {
        temp = value % 16;
        output[i--] = (temp < 10) ? temp + '0' : temp + ('A'-10);
        value = value / 16;
    }
    temp++;
}

void Chip8::ToString(char* output)
{
    const char* namesbuf = "V0 = 0x0000;V1 = 0x0000;V2 = 0x0000;V3 = 0x0000;V4 = 0x0000;V5 = 0x0000;V6 = 0x0000;V7 = 0x0000;V8 = 0x0000;V9 = 0x0000;VA = 0x0000;VB = 0x0000;VC = 0x0000;VD = 0x0000;VE = 0x0000;VF = 0x0000;I  = 0x0000;SP = 0x0000;PC = 0x0000;DT = 0x0000;ST = 0x0000;";

    int values[] = { reg_V[0],reg_V[1],reg_V[2],reg_V[3],reg_V[4],reg_V[5],reg_V[6],reg_V[7],reg_V[8],reg_V[9],reg_V[10],reg_V[11],reg_V[12],reg_V[13], reg_V[14],reg_V[15],reg_I,reg_SP,reg_PC,reg_DT,reg_ST };

    for (size_t i = 0; i < 253; i++)
    {
        output[i] = namesbuf[i];
    }

    for (size_t y = 0; y < 21; y++)
    {
        toHex(&output[(y*12) + 7], values[y],4);
    }
}

