#pragma once
#include "olcPixelGameEngine.h"
#include <iostream>
#include "Display.h"
#define CHIP8_MICROSEC_NOW ((uint64_t)(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()))

class Chip8 : public olc::PixelGameEngine
{
private:
    //registers
    uint8_t             reg_V[16] = {}; 
    uint16_t            reg_I = 0;
    uint8_t             reg_SP = 0;
    uint16_t            reg_PC = 0;
    uint8_t             reg_DT = 0;
    uint8_t             reg_ST = 0;

    //memory
    uint8_t             MEMORY[4096] = {};

    //video 
    Display             display = Display();

    //timing clocks
    uint64_t            m_main_clock = 0;
    uint64_t            m_timers_clock = 0;
    uint64_t            m_instruction_time = 0;

public:
    Chip8();                                                            //  LOADS THE CHIP8 NUMERIC CARACTERS REPRESENTATION IN THE MEMORY 
    ~Chip8();

    //OLC::PIXELGAMEENGINE
    bool OnUserCreate() override;                                       
    bool OnUserUpdate(float fElapsedTime) override;                     //  THE MAIN LOOP

    bool        InitEmulator(const char* fileName);                     //  FIND AND START THE FILE AS WELL AS PREPARE IT TO RUN
                                                                        

    void        soundhandler();

    inline int  loadSprite(const int size, const int x, const int y);   //  LOADS THE SPRITES THAT ARE IN MEMORY TO THE BUFFER OF MY DISPLAY
    void        DrawTheDisplay();                                       //  DRAW THE LODED DISPLAY TO THE WINDOW

private:
    bool        loadFile(const char* fileName);     

    // PART OF THE WORK FLOW
    void        Decode(const uint16_t code);                            //  THE OPCODES EMULATION

    // handle keyboard input
    bool        keyIsPressed(const int key_code);
    uint8_t     waitForKeyPress();

    void        ToString(char* output);                                 

    //  OPCODES
    inline void OP_00E0(const uint16_t code );   // =   CLS
    inline void OP_00EE(const uint16_t code );   // =   RET
    inline void OP_0NNN(const uint16_t code );   // =   JP, nnn (LEGACY-SUPORT) 
    inline void OP_1NNN(const uint16_t code );   // =   JP, nnn
    inline void OP_2NNN(const uint16_t code );   // =   Call, nnn
    inline void OP_3XKK(const uint16_t code );   // =   IF(VX == KK) skip next instruction
    inline void OP_4XKK(const uint16_t code );   // =   IF!(VX == KK) skip next instruction
    inline void OP_5XY0(const uint16_t code );   // =   IF(VX == VY) skip next instruction
    inline void OP_6XKK(const uint16_t code );   // =   MOV VX, KK
    inline void OP_7XKK(const uint16_t code );   // =   ADD VX, KK
    inline void OP_8XY0(const uint16_t code );   // =   MOV VX, VY
    inline void OP_8XY1(const uint16_t code );   // =   VX = VX or VY
    inline void OP_8XY2(const uint16_t code );   // =   VX = VX and VY
    inline void OP_8XY3(const uint16_t code );   // =   VX = VX xor VY
    inline void OP_8XY4(const uint16_t code );   // =   VX = VX + VY    : SET FLAG
    inline void OP_8XY5(const uint16_t code );   // =   VX = VX - VY    : SET FLAG
    inline void OP_8XY6(const uint16_t code );   // =   VX = VX SHR 1   : SET FLAG
    inline void OP_8XY7(const uint16_t code );   // =   VX = VY - VX    : SET FLAG
    inline void OP_8XYE(const uint16_t code );   // =   VX = VX SHL 1   : SET FLAG
    inline void OP_9XY0(const uint16_t code );   // =   IF!(VX == VY) skip next instruction
    inline void OP_ANNN(const uint16_t code );   // =   MOV I, nnn
    inline void OP_BNNN(const uint16_t code );   // =   JP, V0+nnn
    inline void OP_CXKK(const uint16_t code );   // =   MOV VX, (rand & KK)
    inline void OP_DXYN(const uint16_t code );   // =   DRW Vx, Vy, nibble  :   SET FLAG
    inline void OP_EX9E(const uint16_t code );   // =   IF(KEY[VX] == PRESSED) skip next instruction
    inline void OP_EXA1(const uint16_t code );   // =   IF!(KEY[VX] == PRESSED) skip next instruction
    inline void OP_FX07(const uint16_t code );   // =   Vx = DT
    inline void OP_FX0A(const uint16_t code );   // =   WAIT keypress and store in Vx
    inline void OP_FX15(const uint16_t code );   // =   DT = Vx 
    inline void OP_FX18(const uint16_t code );   // =   ST = Vx 
    inline void OP_FX1E(const uint16_t code );   // =   SET I = I + Vx 
    inline void OP_FX29(const uint16_t code );   // =   SET I = NUMBER-VX(sprite)
    inline void OP_FX33(const uint16_t code );   // =   SET I = VX Decimal Numbers
    inline void OP_FX55(const uint16_t code );   // =   PUSH at I
    inline void OP_FX65(const uint16_t code );   // =   POP at I

};

