#pragma once

#include "Color.hpp"
#include "Framebuffer.hpp"

#include <SFML/Graphics.hpp>

class Renderer
{
public:
    Renderer(unsigned int width, unsigned int height);

    void render(sf::RenderWindow& window, const Framebuffer& framebuffer);

private:
    sf::Color toSFML(const Color color);

    sf::Texture texture;
    sf::Sprite sprite;
};

