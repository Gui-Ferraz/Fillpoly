#pragma once

#include "Color.hpp"

#include <vector>

class Framebuffer
{
public:
    Framebuffer(unsigned int width, unsigned int height, Color background = Colors::Black);

    void putPixel(int x, int y, const Color color);
    void clear();

    unsigned int getWidth() const;
    unsigned int getHeight() const;
    const std::vector<Color>& getPixels() const;
    const std::uint8_t* rawPixels() const;
    const Color& getPixel(unsigned int x, unsigned int y) const;
    Color getBackground() const;

    void setBackground(Color color);

private:
    unsigned int width;
    unsigned int height;
    std::vector<Color> pixels;
    Color background;
};