#include "Framebuffer.hpp"

Framebuffer::Framebuffer(unsigned int width, unsigned int height, const Color background)
    : width(width),
    height(height),
    pixels(width* height, background),
    background(background)
{}

void Framebuffer::putPixel(int x, int y, const Color color)
{
    pixels[y * width + x] = color;
}

void Framebuffer::clear()
{
    std::fill(pixels.begin(), pixels.end(), background);
}

unsigned int Framebuffer::getWidth() const
{
    return width;
}

unsigned int Framebuffer::getHeight() const
{
    return height;
}

const std::vector<Color>& Framebuffer::getPixels() const
{
    return pixels;
}

const std::uint8_t* Framebuffer::rawPixels() const
{
    return reinterpret_cast<const std::uint8_t*>(pixels.data());
}

const Color& Framebuffer::getPixel(unsigned int x, unsigned int y) const
{
    return pixels[y * width + x];
}

Color Framebuffer::getBackground() const
{
    return background;
}

void Framebuffer::setBackground(Color color)
{
    background = color;
    Framebuffer::clear();
}
