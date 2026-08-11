#pragma once

#include "Color.hpp"
#include "Point.hpp"

#include<vector>

struct Contour
{
    std::vector<Point> vertices;

    void clear()
    {
        vertices.clear();
    }
    bool empty()
    {
        return vertices.empty();
    }
};

struct Polygon
{
    Contour outer;
    std::vector<Contour> holes;

    Color borderColor = Colors::White;
    Color fillColor = Colors::Black;
    bool colored = false;

    void clear()
    {
        outer.clear();
        for (Contour& hole : holes)
            hole.clear();
    }
};