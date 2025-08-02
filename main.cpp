#include <iostream>
#include "platform.hpp"
#include "chip8.hpp"

int main()
{

    Chip8 chip8;
    Platform platform("cHiP8", &chip8);

    while (platform.isOpen())
    {
        if (chip8.shouldLoad)
        {
            chip8.reset();
            chip8.LoadRom(chip8.romPath);
            chip8.romPath = nullptr;
            chip8.shouldLoad = false;
            chip8.ready = true;
        }

        platform.handleEvents();

        if (chip8.ready)
        {

            platform.processInput(chip8.keypad);

            chip8.Cycle();
            platform.beep();
        }

        platform.display(chip8.screen);
        sf::sleep(sf::milliseconds(16));
    }
}
