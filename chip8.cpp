#include "chip8.hpp"
#include <bits/stdc++.h>

const uint16_t NNN_MASK = 0x0FFFu;
const uint8_t KK_MASK = 0x00ffu;
const uint16_t X_MASK = 0x0f00u;
const uint16_t Y_MASK = 0x00f0u;
const uint8_t N_MASK = 0x000fu;

const int DISPLAY_HEIGHT = 32;
const int DISPLAY_WIDTH = 64;

// Cycle
void Chip8::Cycle()
{

    // Timers
    if (d_timer > 0)
        --d_timer;
    if (s_timer > 0)
        --s_timer;

    for (short i = 0; i < 12; i++)
    {

        // Check for PC bounds BEFORE reading memory
        if (pc < START_ADRESS || pc >= 4095)
        {
            std::cout << "ERROR: PC out of bounds: " << std::hex << pc << std::endl;
            std::cout << "PC should be between " << std::hex << START_ADRESS << " and 4094" << std::endl;
            return;
        }

        uint16_t prevPC = pc; // Track previous PC
        opcode = (memory[pc] << 8u) | memory[pc + 1];
        pc += 2;

        // Debug output
        std::cout << "PC: " << std::hex << prevPC << " -> " << pc << " Opcode: " << std::hex << opcode << std::endl;

        // Special case for opcode 0
        if (opcode == 0)
        {
            std::cout << "WARNING: Executing opcode 0 at PC " << std::hex << prevPC << std::endl;
            std::cout << "Memory at that location: " << std::hex << (int)memory[prevPC] << " " << (int)memory[prevPC + 1] << std::endl;
            return; // Stop execution to prevent infinite loop
        }

        switch (opcode & 0xf000u)
        {
        case 0x0000:
            switch (opcode)
            {
            case 0x00e0:
                OP00E0();
                break;
            case 0x00ee:
                OP00EE();
                break;
            }
            break;

        case 0x1000:
            OP1nnn();
            break;

        case 0x2000:
            OP2nnn();
            break;

        case 0x3000:
            OP3xkk();
            break;

        case 0x4000:
            OP4xkk();
            break;

        case 0x5000:
            OP5xy0();
            break;

        case 0x6000:
            OP6xkk();
            break;

        case 0x7000:
            OP7xkk();
            break;

        case 0x8000:
            switch (opcode & 0x000f)
            {
            case 0x0:
                OP8xy0();
                break;
            case 0x1:
                OP8xy1();
                break;
            case 0x2:
                OP8xy2();
                break;
            case 0x3:
                OP8xy3();
                break;
            case 0x4:
                OP8xy4();
                break;
            case 0x5:
                OP8xy5();
                break;
            case 0x6:
                OP8xy6();
                break;
            case 0x7:
                OP8xy7();
                break;
            case 0xE:
                OP8xyE();
                break;
            }
            break;

        case 0x9000:
            OP9xy0();
            break;

        case 0xA000:
            OPAnnn();
            break;

        case 0xB000:
            OPBnnn();
            break;

        case 0xC000:
            OPCxkk();
            break;

        case 0xd000:
            OPDxyn();
            break;

        case 0xe000:
            switch (opcode & 0x00ff)
            {
            case 0x9e:
                OPEx9E();
                break;
            case 0xa1:
                OPExA1();
                break;
            }
            break;

        case 0xf000:
            switch (opcode & 0x00ff)
            {
            case 0x07:
                OPFx07();
                break;
            case 0x0a:
                OPFx0A();
                break;
            case 0x15:
                OPFx15();
                break;
            case 0x18:
                OPFx18();
                break;
            case 0x1E:
                OPFx1E();
                break;
            case 0x29:
                OPFx29();
                break;
            case 0x33:
                OPFx33();
                break;
            case 0x55:
                OPFx55();
                break;
            case 0x65:
                OPFx65();
                break;
            }
            break;

        default:
            std::cout << "UNIMPLEMENTED OPCODE: " << std::hex << opcode << " at PC: " << (pc - 2) << std::endl;
            std::cout << "Opcode family: " << std::hex << (opcode & 0xF000) << std::endl;

            // Don't crash, just skip for now
            break;
        }
    }
}

void Chip8::LoadRom(char const *filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if (file.is_open())
        {
            std::streampos size = file.tellg();
            char *buffer = new char[size];

            file.seekg(0, std::ios::beg);
            file.read(buffer, size);
            file.close();

            for (long i = 0; i <= size; ++i)
            {
                memory[START_ADRESS + i] = buffer[i];
            }

            delete[] buffer;
        }
        else
        {
            std::cerr << "Falied to load thr ROm file!" << std::endl;
        }
    }
}

// Clear screen
void Chip8::OP00E0()
{
    std::memset(screen, 0, sizeof(screen));
};

// return from subroutine
void Chip8::OP00EE()
{
    --sp;
    pc = stack[sp];
}
// jump to mem location
void Chip8::OP1nnn()
{
    uint16_t nnn = opcode & NNN_MASK;
    pc = nnn;
}

// call a subroutine
void Chip8::OP2nnn()
{
    uint16_t nnn = opcode & NNN_MASK;
    stack[sp] = pc;
    ++sp;
    pc = nnn;
};

// Skip next instruction if Vx = kk.
void Chip8::OP3xkk()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    if (registers[x] == kk)
    {
        pc += 2;
    }
};

// Skip next instruction if Vx != kk.
void Chip8::OP4xkk()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    if (registers[x] != kk)
    {
        pc += 2;
    }
};

// Skip next instruction if Vx = Vy.
void Chip8::OP5xy0()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    if (registers[x] == registers[y])
    {
        pc += 2;
    }
};

// Set Vx = kk.
// The interpreter puts the value kk into register Vx.
void Chip8::OP6xkk()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    registers[x] = kk;
};

// Set Vx = Vx + kk.
// Adds the value kk to the value of register Vx, then stores the result in Vx.
void Chip8::OP7xkk()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    registers[x] += kk;
};

// Set Vx = Vy.
// Stores the value of register Vy in register Vx.
void Chip8::OP8xy0()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    registers[x] = registers[y];
};

// Set Vx = Vx OR Vy.
// Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
void Chip8::OP8xy1()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    registers[x] = registers[x] | registers[y];
};

// Set Vx = Vx AND Vy.
// Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
void Chip8::OP8xy2()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    registers[x] = registers[x] & registers[y];
};

// Set Vx = Vx XOR Vy.
// Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
void Chip8::OP8xy3()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    registers[x] = registers[x] ^ registers[y];
};

// 8xy4 - ADD Vx, Vy
// Set Vx = Vx + Vy, set VF = carry.
// The values of Vx and Vy are added together. If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are kept, and stored in Vx.

void Chip8::OP8xy4()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    uint16_t sum = registers[x] + registers[y];

    registers[x] = sum & 0xff;

    if (sum > 0xffu)
        registers[0xfu] = 1;
    else
        registers[0xfu] = 0;
};

// 8xy5 - SUB Vx, Vy
// Set Vx = Vx - Vy, set VF = NOT borrow.
// If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
void Chip8::OP8xy5()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;

    uint8_t diff = registers[x] - registers[y];

    if (registers[x] >= registers[y])
    {
        registers[x] = diff;

        registers[0xf] = 1;
    }
    else
    {
        registers[x] = diff;

        registers[0xf] = 0;
    }
};

// 8xy6 - SHR Vx {, Vy}
// Set Vx = Vx SHR 1.
// If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is divided by 2.
void Chip8::OP8xy6()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;
    registers[x] = registers[y];
    uint8_t temp = registers[x] & 0x1u;
    registers[x] >>= 1;
    registers[0xF] = temp;
}

// 8xy7 - SUBN Vx, Vy
// Set Vx = Vy - Vx, set VF = NOT borrow.
// If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the results stored in Vx.
void Chip8::OP8xy7()
{
    uint8_t x = (opcode & X_MASK) >> 8;
    uint8_t y = (opcode & Y_MASK) >> 4;

    // registers[y] > registers[x] ? registers[0xf] = 1 : registers[0xf] = 0;
    registers[x] = registers[y] - registers[x];

    if (registers[y] >= registers[x])
        registers[0xfu] = 1;
    else
        registers[0xfu] = 0;
};

// 8xyE - SHL Vx {, Vy}
// Set Vx = Vx SHL 1.
// If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx is multiplied by 2
void Chip8::OP8xyE()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t y = (opcode & Y_MASK) >> 4u;
    registers[x] = registers[y];
    uint8_t msb = (registers[x] & 0x80u) >> 7u;
    registers[x] <<= 1;
    registers[0xf] = msb;
};

// 9xy0 - SNE Vx, Vy
// Skip next instruction if Vx != Vy.
// The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
void Chip8::OP9xy0()
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
void Chip8::OPAnnn()
{
    uint16_t nnn = opcode & NNN_MASK;
    IR = nnn;
};

// Bnnn - JP V0, addr
// Jump to location nnn + V0.
// The program counter is set to nnn plus the value of V0.
void Chip8::OPBnnn()
{
    uint16_t nnn = opcode & NNN_MASK;
    pc = nnn + registers[0];
};

// Cxkk - RND Vx, byte
// Set Vx = random byte AND kk.
// The interpreter generates a random number from 0 to 255, which is then ANDed with the value kk. The results are stored in Vx. See instruction 8xy2 for more information on AND.
void Chip8::OPCxkk()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t kk = opcode & KK_MASK;

    uint8_t randomVal = randByte(randGen);

    registers[x] = kk & randomVal;
};

// Dxyn - DRW Vx, Vy, nibble
// Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

// The interpreter reads n bytes from memory, starting at the address stored in I. These bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the coordinates of the display, it wraps around to the opposite side of the screen. See instruction 8xy3 for more information on XOR, and section 2.4, Display, for more information on the Chip-8 screen and sprites.
void Chip8::OPDxyn()
{

    uint8_t _x = (opcode & X_MASK) >> 8u;
    uint8_t _y = (opcode & Y_MASK) >> 4u;

    uint8_t x = registers[_x] % DISPLAY_WIDTH;
    uint8_t y = registers[_y] % DISPLAY_HEIGHT;

    uint8_t height = opcode & 0x000F;

    registers[0xF] = 0; // Reset collision flag

    for (uint8_t row = 0; row < height; ++row)
    {
        uint8_t spriteByte = memory[IR + row];

        for (uint8_t col = 0; col < 8; ++col)
        {
            // Extract the pixel bit (either 0 or 1)
            uint8_t spritePixel = (spriteByte >> (7 - col)) & 0x1;

            // Wrap around if x + col >= DISPLAY_WIDTH
            uint32_t posX = (x + col) % DISPLAY_WIDTH;
            uint32_t posY = (y + row) % DISPLAY_HEIGHT;

            uint32_t index = posY * DISPLAY_WIDTH + posX;

            if (spritePixel)
            {
                // If the screen pixel is already set, we have a collision
                if (screen[index] == 0xFFFFFFFF)
                    registers[0xF] = 1;

                // XOR the pixel (toggle on/off)
                screen[index] ^= 0xFFFFFFFF;
            }
        }
    }
}

// Ex9E - SKP Vx
// Skip next instruction if key with the value of Vx is pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the down position, PC is increased by 2.
void Chip8::OPEx9E()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t key = registers[x];

    if (keypad[key & 0xF])
    {
        pc += 2;
    }
};

// ExA1 - SKNP Vx
// Skip next instruction if key with the value of Vx is not pressed.
// Checks the keyboard, and if the key corresponding to the value of Vx is currently in the up position, PC is increased by 2.
void Chip8::OPExA1()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t key = registers[x];

    if (!keypad[key & 0xF])
    {
        pc += 2;
    }
};

// Fx07 - LD Vx, DT
// Set Vx = delay timer value.
// The value of DT is placed into Vx.
void Chip8::OPFx07()
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    registers[x] = d_timer;
};

// Fx0A - LD Vx, K
// Wait for a key press, store the value of the key in Vx.

// All execution stops until a key is pressed, then the value of that key is stored in Vx.
void Chip8::OPFx0A()
{
    uint8_t x = X_MASK >> 8u;

    if (keypad[0])
    {
        registers[x] = 0;
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
void Chip8::OPFx15()
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    d_timer = registers[x];
};

// Fx18 - LD ST, Vx
// Set sound timer = Vx.

// ST is set equal to the value of Vx.
void Chip8::OPFx18()
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    s_timer = registers[x];
};

// Fx1E - ADD I, Vx
// Set I = I + Vx.

// The values of I and Vx are added, and the results are stored in I.
void Chip8::OPFx1E()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    IR += registers[x];
};

// Fx29 - LD F, Vx
// Set I = location of sprite for digit Vx.
// The value of I is set to the location for the hexadecimal sprite corresponding to the value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal font.
void Chip8::OPFx29()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t digit = registers[x];

    IR = FONTSET_START_ADDRESS + (5 * digit);
};

// Fx33 - LD B, Vx
// Store BCD representation of Vx in memory locations I, I+1, and I+2.
// The interpreter takes the decimal value of Vx, and places the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.
void Chip8::OPFx33()
{
    uint8_t x = (opcode & X_MASK) >> 8u;
    uint8_t val = registers[x];

    // Ones place
    memory[IR + 2] = val % 10;
    val /= 10;

    // tens
    memory[IR + 1] = val % 10;
    val /= 10;

    // hundreds
    memory[IR] = val % 10;
};

// Fx55 - LD [I], Vx
// Store registers V0 through Vx in memory starting at location I.

// The interpreter copies the values of registers V0 through Vx into memory, starting at the address in I.

void Chip8::OPFx55()
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    for (uint8_t i = 0; i <= x; ++i)
    {
        memory[IR + i] = registers[i];
    }
};

// Fx65 - LD Vx, [I]
// Read registers V0 through Vx from memory starting at location I.
// The interpreter reads values from memory starting at location I into registers V0 through Vx.
void Chip8::OPFx65()
{
    uint8_t x = (opcode & X_MASK) >> 8u;

    for (uint8_t i = 0; i <= x; ++i)
    {
        registers[i] = memory[IR + i];
    }
}