#pragma once

#include "Framebuffer.hpp"
#include "Point.hpp"
#include "Polygon.hpp"

class Rasterizer
{
public:
	static void drawPixel(Framebuffer& framebuffer, int x, int y, Color color);
	static void drawLine(Framebuffer& framebuffer, Point start, Point end, Color color);
    static void drawContour(Framebuffer& framebuffer, const Contour& contour, Color color);
    static void drawPolygon(Framebuffer& framebuffer, Polygon& polygon, Color color = Colors::White);
	static void fillpoly(Framebuffer& framebuffer, Polygon& polygon, Color color = Colors::Black);
    //static void uncolorPolygon(Framebuffer& framebuffer, const Polygon& polygon);
    static bool pointInContour(const Point& point, const Contour& contour);
    static bool pointInPolygon(const Point& point, const Polygon& polygon);

private:
    struct Edge
    {
        int yMin;
        int yMax;
        double x;
        double inverseSlope;
    };

    struct EdgeTable
    {
        int minY;
        int maxY;
        std::vector<std::vector<Edge>> edges;
    };

    static std::vector<Edge> getEdges(const std::vector<Point>& vertices);
    static std::vector<Edge> getAllEdges(const Polygon& polygon);
    static EdgeTable buildEdgeTable(const Polygon& polygon);
};

