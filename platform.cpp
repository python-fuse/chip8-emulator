#include <iostream>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "platform.hpp"
#include "tinyfile/tinyfiledialogs.h"

using namespace sf;
using namespace sf::Keyboard;

Platform::Platform(std::string title, Chip8 *chip) : window(sf::VideoMode({static_cast<unsigned int>(windowWidth * pixelSize), static_cast<unsigned int>(windowHeight * pixelSize)}), title), sBuffer("assets/beep.mp3"), s(sBuffer)
{
    this->chip = chip;
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window, true);
}

Platform::~Platform()
{
    window.close();
    ImGui::SFML::Shutdown();
}

void Platform::handleEvents()
{
    while (const std::optional event = window.pollEvent())
    {
        ImGui::SFML::ProcessEvent(window, *event);
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
    ImGui::SFML::Update(window, deltaClock.restart());

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Load ROM"))
            {
                // Set a flag to open file dialog or hardcoded loader
                // loadRom("roms/INVADERS"); // or use std::ifstream picker

                char *loadedFilePath = Platform::pickFile();

                std::cout << "File loaded" << std::endl;
                chip->romPath = loadedFilePath;
                chip->shouldLoad = true;
            }
            if (ImGui::MenuItem("Exit"))
            {
                window.close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

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

    ImGui::SFML::Render(window);

    window.display();
}

char *Platform::pickFile()
{
    const char *filters[] = {"*.ch8", "*.c8b"};

    char *filePath = tinyfd_openFileDialog(
        "Select a CHIP8 ROM",
        "~/Desktop",
        2,
        filters, "Chip8 .ch8 or .c8b files.", 0);

    return filePath;
}

void Platform::processInput(uint8_t *keys)
{
    for (int i = 0; i < 16; i++)
        keys[i] = 0;

    if (isKeyPressed(Scan::Num1))
        keys[0x1] = 1;
    if (isKeyPressed(Scan::Num2))
        keys[0x2] = 1;
    if (isKeyPressed(Scan::Num3))
        keys[0x3] = 1;
    if (isKeyPressed(Scan::Num4))
        keys[0xc] = 1;

    if (isKeyPressed(Scan::Q))
        keys[0x4] = 1;
    if (isKeyPressed(Scan::W))
        keys[0x5] = 1;
    if (isKeyPressed(Scan::E))
        keys[0x6] = 1;
    if (isKeyPressed(Scan::R))
        keys[0xd] = 1;

    if (isKeyPressed(Scan::A))
        keys[0x7] = 1;
    if (isKeyPressed(Scan::S))
        keys[0x8] = 1;
    if (isKeyPressed(Scan::D))
        keys[0x9] = 1;
    if (isKeyPressed(Scan::F))
        keys[0xe] = 1;

    if (isKeyPressed(Scan::Z))
        keys[0xa] = 1;
    if (isKeyPressed(Scan::X))
        keys[0x0] = 1;
    if (isKeyPressed(Scan::C))
        keys[0xb] = 1;
    if (isKeyPressed(Scan::V))
        keys[0xf] = 1;
}

void Platform::beep()
{

    if (chip->s_timer > 0)
    {
        s.play();
    }
}