#include <iostream>
#include "platform.hpp"
#include "chip8.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << "<Rom>\n";
        std::exit(EXIT_FAILURE);
        return -1;
    }

    Platform platform("cHiP8");
    Chip8 chip8;

    chip8.LoadRom(argv[1]);

    while (platform.isOpen())
    {
        platform.handleEvents();
        platform.processInput(chip8.keypad);

        chip8.Cycle();

        platform.display(chip8.screen);
        sf::sleep(sf::milliseconds(16));
    }
}
