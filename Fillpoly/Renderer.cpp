#include "Renderer.hpp"

Renderer::Renderer(unsigned int width, unsigned int height)
    : texture(sf::Vector2(width, height)),
    sprite(texture)
{}

void Renderer::render(sf::RenderWindow& window, const Framebuffer& framebuffer)
{
    texture.update(framebuffer.rawPixels());
    sprite.setTexture(texture);
    window.draw(sprite);
}

sf::Color Renderer::toSFML(const Color color)
{
    return { color.r, color.g, color.b, color.a };
}