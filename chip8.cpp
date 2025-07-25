#include "chip8.hpp"
#include <bits/stdc++.h>

// Template for opcode functions
#define DEF_OP(name) void Chip8::name()

const uint16_t NNN_MASK = 0x0FFFu;
const uint8_t KK_MASK = 0x00ffu;
const uint16_t X_MASK = 0x0f00u;
const uint16_t Y_MASK = 0x00f0u;
const uint8_t N_MASK = 0x000fu;

const int DISPLAY_HEIGHT(32);
const int DISPLAY_WIDTH(64);

// Clear screen
DEF_OP(OP00E0)
{
    std::memset(screen, 0, sizeof(Chip8::screen));
};

// return from subroutine
DEF_OP(OP00EE)
{
    --sp;
    pc = stack[sp];
}
// jump to mem location
DEF_OP(OP1nnn)
{
    uint16_t nnn = opcode & NNN_MASK;
    pc = nnn;
}

// call a subroutine
DEF_OP(OP2nnn)
{
    uint16_t nnn = opcode & NNN_MASK;
    stack[0] = pc;
    pc = nnn;
};

// Skip next instruction if Vx = kk.
DEF_OP(OP3xkk)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    if (x == kk)
    {
        pc += 2;
    }
};

// Skip next instruction if Vx != kk.
DEF_OP(OP4xkk)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    if (x != kk)
    {
        pc += 2;
    }
};

// Skip next instruction if Vx = Vy.
DEF_OP(OP5xy0)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    if (x == y)
    {
        pc += 2;
    }
};

// Set Vx = kk.
// The interpreter puts the value kk into register Vx.
DEF_OP(OP6xkk)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    registers[x] = kk;
};

// Set Vx = Vx + kk.
// Adds the value kk to the value of register Vx, then stores the result in Vx.
DEF_OP(OP7xkk)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    registers[x] += kk;
};

// Set Vx = Vy.
// Stores the value of register Vy in register Vx.
DEF_OP(OP8xy0)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    registers[x] = registers[y];
};

// Set Vx = Vx OR Vy.
// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
DEF_OP(OP8xy1)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    registers[x] = registers[x] | registers[y];
};

// Set Vx = Vx AND Vy.
// Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
DEF_OP(OP8xy2)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    registers[x] = registers[x] & registers[y];
};

// Set Vx = Vx XOR Vy.
// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
DEF_OP(OP8xy3)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    registers[x] = registers[x] ^ registers[y];
};

// 8xy4 - ADD Vx, Vy
// Set Vx = Vx + Vy, set VF = carry.
// The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.

DEF_OP(OP8xy4)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    uint16_t sum = registers[x] + registers[y];

    if (sum > 0xffu)
        registers[0xfu] = 1;
    else
        registers[0xfu] = 0;

    registers[x] = sum & 0xff;
};

// 8xy5 - SUB Vx, Vy
// Set Vx = Vx - Vy, set VF = NOT borrow.
// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
DEF_OP(OP8xy5)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    if (registers[x] > registers[y])
        registers[0xf] = 1;
    else
        registers[0xf] = 0;

    uint8_t diff = registers[x] - registers[y];
    registers[x] = diff;
};

// 8xy6 - SHR Vx {, Vy}
// Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
DEF_OP(OP8xy6)
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    registers[0xf] = registers[x] & 1;

    registers[x] >>= 1;
};

// 8xy7 - SUBN Vx, Vy
// Set Vx = Vy - Vx, set VF = NOT borrow.
// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
DEF_OP(OP8xy7)
{
    uint8_t x = (opcode & X_MASK) >> 8;
    uint8_t y = (opcode & Y_MASK) >> 4;

    registers[y] > registers[x] ? registers[0xf] = 1 : registers[0xf] = 0;

    registers[x] = registers[y] - registers[x];
};

// 8xyE - SHL Vx {, Vy}
// Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2
DEF_OP(OP8xyE)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t msb = (registers[x] & 0x80) >> 7;

    registers[0xf] = msb;

    registers[x] <<= 1;
};

// 9xy0 - SNE Vx, Vy
// Skip next instruction if Vx != Vy.
// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
DEF_OP(OP9xy0)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    if (registers[x] != registers[y])
    {
        pc += 2;
    }
};

// Annn - LD I, addr
// Set I = nnn.
// The value of register I is set to nnn.
DEF_OP(OPAnnn)
{
    uint16_t nnn = opcode & NNN_MASK;
    I = nnn;
};

// Bnnn - JP V0, addr
// Jump to location nnn + V0.
// The program counter is set to nnn plus the value of V0.
DEF_OP(OPBnnn)
{
    uint16_t nnn = opcode & NNN_MASK;
    pc = nnn + registers[0];
};

// Cxkk - RND Vx, byte
// Set Vx = random byte AND kk.
// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
DEF_OP(OPCxkk)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    uint8_t randomVal = randByte(randGen);

    registers[x] = kk & randomVal;
};

// Dxyn - DRW Vx, Vy, nibble
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

// The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
DEF_OP(OPDxyn)
{
    uint8_t _x = (opcode & X_MASK) >> 8u;
    uint8_t _y = (opcode & Y_MASK) >> 4u;

    uint8_t x(registers[_x] % DISPLAY_WIDTH);
    uint8_t y(registers[_y] % DISPLAY_HEIGHT);

    registers[0xf] = 0;

    uint8_t height = opcode & 0x000F;

    for (uint8_t row = 0; row < height; ++row)
    {
        uint8_t spriteByte = memory[I + row];

        for (uint8_t col(0); col < 8; ++col)
        {
            uint8_t spritePixel = (spriteByte & (0x80 >> col));
            uint32_t *screenPixel = &screen[(y + row) * DISPLAY_WIDTH + (x + col)];

            if (spritePixel)
            {
                if (*screenPixel)
                {
                    registers[0xf] = 1;
                }

                *screenPixel ^= 0xffffffff;
            }
        }
    }
};

// Ex9E - SKP Vx
// Skip next instruction if key with the value of Vx is pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
DEF_OP(OPEx9E)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t key = registers[x];

    if (keypad[key])
    {
        pc += 2;
    }
};

// ExA1 - SKNP Vx
// Skip next instruction if key with the value of Vx is not pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
DEF_OP(OPExA1)
{
    uint8_t x = (opcode & X_MASK) >> 8;
    uint8_t key = registers[x];

    if (!keypad[key])
    {
        pc += 2;
    }
};

// Fx07 - LD Vx, DT
// Set Vx = delay timer value.
// The value of DT is placed into Vx.
DEF_OP(OPFx07)
{
    uint8_t x = (opcode & X_MASK) >> 8;

    registers[x] = d_timer;
};

// Fx0A - LD Vx, K
// Wait for a key press, store the value of the key in Vx.

// All execution stops until a key is pressed, then the value of that key is stored in Vx.
DEF_OP(OPFx0A)
{
    uint8_t x = X_MASK >> 8u;

    if (keypad[0])
    {
        registers[x] = 1;
    }
    else if (keypad[1])
    {
        registers[x] = 1;
    }
    else if (keypad[2])
    {
        registers[x] = 2;
    }
    else if (keypad[3])
    {
        registers[x] = 3;
    }
    else if (keypad[4])
    {
        registers[x] = 4;
    }
    else if (keypad[5])
    {
        registers[x] = 5;
    }
    else if (keypad[6])
    {
        registers[x] = 6;
    }
    else if (keypad[7])
    {
        registers[x] = 7;
    }
    else if (keypad[8])
    {
        registers[x] = 8;
    }
    else if (keypad[9])
    {
        registers[x] = 9;
    }
    else if (keypad[10])
    {
        registers[x] = 10;
    }
    else if (keypad[11])
    {
        registers[x] = 11;
    }
    else if (keypad[12])
    {
        registers[x] = 12;
    }
    else if (keypad[13])
    {
        registers[x] = 13;
    }
    else if (keypad[14])
    {
        registers[x] = 14;
    }
    else if (keypad[15])
    {
        registers[x] = 15;
    }
    else
    {
        pc -= 2;
    }
};

// Fx15 - LD DT, Vx
// Set delay timer = Vx.

// DT is set equal to the value of Vx.
DEF_OP(OPFx15)
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    d_timer = registers[x];
};

// Fx18 - LD ST, Vx
// Set sound timer = Vx.

// ST is set equal to the value of Vx.
DEF_OP(OPFx18)
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    s_timer = registers[x];
};

// Fx1E - ADD I, Vx
// Set I = I + Vx.

// The values of I and Vx are added, and the results are stored in I.
DEF_OP(OPFx1E)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    I += registers[x];
};

// Fx29 - LD F, Vx
// Set I = location of sprite for digit Vx.
// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal font.
DEF_OP(OPFx29)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t digit = registers[x];

    I = FONTSET_START_ADDRESS + (5 * digit);
};

// Fx33 - LD B, Vx
// Store BCD representation of Vx in memory locations I, I+1, and I+2.
// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
DEF_OP(OPFx33)
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t val = registers[x];

    // Ones place
    memory[I + 2] = val % 10;
    val /= 10;

    // tens
    memory[I + 1] = val % 10;
    val /= 10;

    // hundreds
    memory[I] = val % 10;
};

// Fx55 - LD [I], Vx
// Store registers V0 through Vx in memory starting at location I.

// The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.

DEF_OP(OPFx55)
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    for (uint8_t i = 0; i < x; ++I)
    {
        memory[I + i] = registers[i];
    }
};

// Fx65 - LD Vx, [I]
// Read registers V0 through Vx from memory starting at location I.
// The interpreter reads values from memory starting at location I into registers V0 through Vx.
DEF_OP(OPFx65)
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    for (uint8_t i = 0; i <= x; ++i)
    {
        registers[i] = memory[I + i];
    }
}