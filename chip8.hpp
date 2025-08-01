#ifndef CHIP_8_H
#define CHIP_8_H

#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>
#include <bits/stdc++.h>

struct Chip8
{

    // Memory
    uint8_t memory[4096];
    // Register v1 - vf
    uint8_t registers[16];

    // I Register
    uint16_t IR;

    // PC Register
    uint16_t pc;

    const unsigned int DISPLAY_WIDTH = 64;
    const unsigned int DISPLAY_HEIGHT = 32;

    // Stack poiter
    uint8_t sp;

    // Stack
    uint16_t stack[16];

    // Keys
    uint8_t keypad[16];

    // video
    uint32_t screen[64 * 32];

    // dealay + soud timer
    uint8_t d_timer;
    uint8_t s_timer;

    // opcode
    uint16_t opcode;

    // Load rom flag + rom path
    bool shouldLoad = false;
    char *romPath;

    bool ready = false;

    // draw flag

    const unsigned int START_ADRESS = 0x200;

    const unsigned int FONTSET_START_ADDRESS = 0x50;

    Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count())
    {
        pc = START_ADRESS;

        // Initialize all other arrays
        reset();

        // Reset other variables
        IR = 0;
        sp = 0;
        d_timer = 0;
        s_timer = 0;

        // rg
        randByte = std::uniform_int_distribution<uint8_t>(0, 255);
    }

    std::default_random_engine randGen;
    std::uniform_int_distribution<uint8_t> randByte;

    void reset()
    {
        memset(screen, 0, sizeof(screen));
        memset(memory, 0, sizeof(memory));
        memset(registers, 0, sizeof(registers));
        memset(keypad, 0, sizeof(keypad));
        memset(stack, 0, sizeof(stack));
        pc = START_ADRESS;

        for (unsigned int i = 0; i < 80; ++i)
        {
            memory[FONTSET_START_ADDRESS + i] = fontset[i];
        }
    }

    uint8_t fontset[80] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    void LoadRom(char const *filename);
    void Cycle();

    // OPCODES
    void OP00E0();
    void OP00EE();
    void OP1nnn();
    void OP2nnn();
    void OP3xkk();
    void OP4xkk();
    void OP5xy0();
    void OP6xkk();
    void OP7xkk();
    void OP8xy0();
    void OP8xy1();
    void OP8xy2();
    void OP8xy3();
    void OP8xy4();
    void OP8xy5();
    void OP8xy6();
    void OP8xy7();
    void OP8xyE();
    void OP9xy0();
    void OPAnnn();
    void OPBnnn();
    void OPCxkk();
    void OPDxyn();
    void OPEx9E();
    void OPExA1();
    void OPFx07();
    void OPFx0A();
    void OPFx15();
    void OPFx18();
    void OPFx1E();
    void OPFx29();
    void OPFx33();
    void OPFx55();
    void OPFx65();
};

#endif