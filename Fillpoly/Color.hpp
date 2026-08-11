#pragma once

#include <cstdint>

// RGBA -> 4 Bytes
struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a = 255;  // Alpha -> transparency

    bool operator==(const Color& other) const
    {
        return (r == other.r && g == other.g && b == other.b && a == other.a);
    }
};

namespace Colors
{
    inline constexpr Color Black{ 0, 0, 0 };
    inline constexpr Color White{ 255, 255, 255 };
    inline constexpr Color Red{ 255, 0, 0 };
    inline constexpr Color Green{ 0, 255, 0 };
    inline constexpr Color Blue{ 0, 0, 255 };
}
