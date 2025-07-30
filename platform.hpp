#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "chip8.hpp"

class Platform
{
private:
    const int windowWidth = 64;
    const int windowHeight = 32;
    const int pixelSize = 10;
    sf::RenderWindow window;
    sf::Clock deltaClock;

    std::string soundPath = "assets/beep.mp3";

    sf::SoundBuffer sBuffer;
    sf::Sound s;

    Chip8 *chip;

public:
    Platform(std::string title, Chip8 *chip);
    ~Platform();

    void handleEvents();
    void clear();
    void display(void const *buffer);
    bool isOpen() const;
    void processInput(uint8_t *keys);
    void beep();
};
