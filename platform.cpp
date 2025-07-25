#include <iostream>
#include "platform.hpp"

using namespace sf;

Platform::Platform(std::string title) : window(sf::VideoMode({windowWidth * pixelSize, windowHeight * pixelSize}), title)
{
    window.setFramerateLimit(60);

    // Debug output
    std::cout << "Platform constructor - windowWidth: " << windowWidth << ", windowHeight: " << windowHeight << std::endl;
    std::cout << "Window size: " << windowWidth * pixelSize << "x" << windowHeight * pixelSize << std::endl;
}

Platform::~Platform()
{
    window.close();
}

void Platform::handleEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
    }
}

void Platform::clear()
{
    window.clear(sf::Color::Black);
}

bool Platform::isOpen() const
{
    return window.isOpen();
}
void Platform::display(void const *buffer)
{
    std::cout << "Displaying frame\n";

    const uint32_t *pixels = static_cast<const uint32_t *>(buffer);

    window.clear();
    sf::RectangleShape pixelShape({(float)pixelSize, (float)pixelSize});
    pixelShape.setFillColor(sf::Color::White);

    // Fixed: Use windowWidth and windowHeight directly (64x32)
    for (int y = 0; y < windowHeight; ++y)
    {
        for (int x = 0; x < windowWidth; ++x)
        {
            if (pixels[y * windowWidth + x])
            {
                pixelShape.setPosition(sf::Vector2<float>(x * pixelSize, y * pixelSize));
                window.draw(pixelShape);
            }
        }
    }

    window.display();
}
void Platform::processInput(uint8_t *keys)
{
    for (int i = 0; i < 16; i++)
        keys[i] = 0;

    if (Keyboard::isKeyPressed(Keyboard::Scan::Num1))
        keys[0x1] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::Num2))
        keys[0x2] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::Num3))
        keys[0x3] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::Num4))
        keys[0xc] = 1;

    if (Keyboard::isKeyPressed(Keyboard::Scan::Q))
        keys[0x4] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::W))
        keys[0x5] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::E))
        keys[0x6] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::R))
        keys[0xd] = 1;

    if (Keyboard::isKeyPressed(Keyboard::Scan::A))
        keys[0x7] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::S))
        keys[0x8] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::D))
        keys[0x9] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::F))
        keys[0xe] = 1;

    if (Keyboard::isKeyPressed(Keyboard::Scan::Z))
        keys[0xa] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::X))
        keys[0x0] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::C))
        keys[0xb] = 1;
    if (Keyboard::isKeyPressed(Keyboard::Scan::V))
        keys[0xf] = 1;
}