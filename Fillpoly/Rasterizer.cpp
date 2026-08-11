#include "Rasterizer.hpp"

#include <algorithm>
#include <cmath>

void Rasterizer::drawPixel(Framebuffer& framebuffer, int x, int y, Color color)
{
	framebuffer.putPixel(x, y, color);
}

// Bresenham Algorithm
void Rasterizer::drawLine(Framebuffer& framebuffer, Point start, Point end, Color color)
{
	int dx = abs(end.x - start.x);
	int dy = abs(end.y - start.y);

	int sx = 1;
	if (start.x > end.x)
	{
		sx = -1;
	}

	int sy = 1;
	if (start.y > end.y)
	{
		sy = -1;
	}

	int err = dx - dy;

	while (true)
	{
		drawPixel(framebuffer, start.x, start.y, color);

		if (start.x == end.x && start.y == end.y)
			break;

		int e2 = 2 * err;

		if (e2 > -dy)
		{
			err = err - dy;
			start.x = start.x + sx;
		}
		if (e2 < dx)
		{
			err = err + dx;
			start.y = start.y + sy;
		}
	}
}

void Rasterizer::drawContour(Framebuffer& framebuffer, const Contour& contour, Color color)
{
	size_t size = contour.vertices.size();
	for (size_t i = 0; i < size; ++i)
	{
		size_t j = (i + 1) % size;
		drawLine(framebuffer, contour.vertices[i],contour.vertices[j], color);
	}
}

void Rasterizer::drawPolygon(Framebuffer& framebuffer, Polygon& polygon, Color color)
{
	if (polygon.colored)
	{
		fillpoly(framebuffer, polygon, polygon.fillColor);
	}

	drawContour(framebuffer, polygon.outer, color);

	for (const Contour& hole : polygon.holes)
	{
		drawContour(framebuffer, hole, color);
	}

	polygon.borderColor = color;
}

void Rasterizer::fillpoly(Framebuffer& framebuffer, Polygon& polygon, Color color)
{
	EdgeTable table = buildEdgeTable(polygon);

	std::vector<Edge> activeEdges;

	for (int y = table.minY; y < table.maxY; ++y)
	{
		// 1. Add edges that starts in y
		for (const Edge& edge : table.edges[y - table.minY])
		{
			activeEdges.push_back(edge);
		}

		// 2. if y >= yMax, the esdge is no longer active
		auto outOfScanline = std::remove_if(activeEdges.begin(),
			activeEdges.end(),
			[y](const Edge& edge)
			{
				return y >= edge.yMax;
			});	// after remove_if: vector = [edgesInScanline | edgesOutOfScanline]

		activeEdges.erase(outOfScanline, activeEdges.end());	// erase(first position, last position)

		// 3. Sort by edge.x
		std::sort(activeEdges.begin(), activeEdges.end(), [](const Edge& a, const Edge& b)
			{
				return a.x < b.x;
			});

		// 4. Fill intersections
		for (size_t i = 0; i + 1 < activeEdges.size(); i += 2)
		{
			int xStart = static_cast<int>(std::ceil(activeEdges[i].x));
			int xEnd = static_cast<int>(std::floor(activeEdges[i + 1].x));

			for (int x = xStart; x <= xEnd; ++x)
			{
				framebuffer.putPixel(x, y, color);
			}
		}

		// 5. Update x
		for (Edge& edge : activeEdges)
		{
			edge.x += edge.inverseSlope;
		}
	}
	polygon.fillColor = color;
	polygon.colored = true;
}
/*
void Rasterizer::uncolorPolygon(Framebuffer& framebuffer, Polygon& polygon)
{
	fillpoly(framebuffer, polygon, { 0, 0, 0, 0 });
	polygon.colored = false;
}
*/
bool Rasterizer::pointInContour(const Point& point, const Contour& contour)
{
	bool inside = false;

	const auto& vertices = contour.vertices;

	for (size_t i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++)
	{
		const Point& p1 = vertices[i];
		const Point& p2 = vertices[j];

		if ((p1.y > point.y) != (p2.y > point.y))
		{
			double intersectionX = p1.x + (point.y - p1.y) * (p2.x - p1.x) /
				static_cast<double>(p2.y - p1.y);

			if (point.x < intersectionX)
			{
				inside = !inside;
			}
		}
	}

	return inside;
}

bool Rasterizer::pointInPolygon(const Point& point, const Polygon& polygon)
{
	if (!pointInContour(point, polygon.outer))
	{
		return false;
	}

	for (const Contour& hole : polygon.holes)
	{
		if (pointInContour(point, hole))
		{
			return false;
		}
	}

	return true;
}

std::vector<Rasterizer::Edge> Rasterizer::getEdges(const std::vector<Point>& vertices)
{
	std::vector<Edge> edges;

	for (size_t i = 0; i < vertices.size(); ++i)
	{
		size_t j = (i + 1) % vertices.size();	// if i == vertices.size() - 1, then j = 0, else j = i + 1

		Point p1 = vertices[i];
		Point p2 = vertices[j];

		if (p1.y > p2.y)
			std::swap(p1, p2);

		if (p1.y == p2.y)
			continue;

		Edge edge;

		edge.yMin = p1.y;
		edge.yMax = p2.y;
		edge.x = p1.x;

		double dx = p2.x - p1.x;
		double dy = p2.y - p1.y;
		edge.inverseSlope = dx / dy;

		edges.push_back(edge);
	}
	
	return edges;
}

std::vector<Rasterizer::Edge> Rasterizer::getAllEdges(const Polygon& polygon)
{
	std::vector<Edge> edges;

	auto outerEdges = getEdges(polygon.outer.vertices);

	edges.insert(edges.end(), outerEdges.begin(), outerEdges.end());

	for (const Contour& hole : polygon.holes)
	{
		auto holeEdges = getEdges(hole.vertices);

		edges.insert(edges.end(), holeEdges.begin(), holeEdges.end());
	}

	return edges;
}

Rasterizer::EdgeTable Rasterizer::buildEdgeTable(const Polygon& polygon)
{
	std::vector<Edge> edges = getAllEdges(polygon);

	int minY = edges[0].yMin;
	int maxY = edges[0].yMax;

	for (const Edge& edge : edges)
	{
		if (edge.yMin < minY)
			minY = edge.yMin;
		if (edge.yMax > maxY)
			maxY = edge.yMax;
	}

	EdgeTable table;
	table.minY = minY;
	table.maxY = maxY;
	table.edges.resize(maxY - minY);

	for (const Edge& edge : edges)
	{
		table.edges[edge.yMin - minY].push_back(edge);
	}

	return table;
}