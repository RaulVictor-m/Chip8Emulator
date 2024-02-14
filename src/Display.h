#pragma once
#include <iostream>
#include <cstdint>

class Display
{
public:
    static const uint32_t SCREEN_HEIGHT = 32;
    static const uint32_t SCREEN_WIDTH = 64;

    const uint8_t& operator[](const int index) const;

    void cls();
    bool loadSprite(const uint8_t* sprite, const int size, int x, int y);

private:
    uint8_t m_display[SCREEN_WIDTH * SCREEN_HEIGHT] = {};
};

