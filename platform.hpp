#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Platform
{
private:
    sf::RenderWindow window;
    const int windowWidth = 64;
    const int windowHeight = 32;
    const int pixelSize = 10;

public:
    Platform(std::string title);
    ~Platform();

    void handleEvents();
    void clear();
    void display(void const *buffer);
    bool isOpen() const;
    void processInput(uint8_t *keys);
};
