#ifndef POLYGON_H
#define POLYGON_H

#include <SDL2/SDL.h>
#include "Shape.h"
#include "Point.h"
#include "Primitives.h"
#include <vector>

class Polygon : public Shape {
private:
    std::vector<Point> points;
    Uint32 color;

public:
    Polygon() : color(0xFFFFFFFF) {} // Construtor padrão

    Polygon(const std::vector<Point>& points, Uint32 color)
        : points(points), color(color) {}

    void draw(SDL_Surface* surface) override {
        if (!surface) return;

        for (size_t i = 0; i < points.size() - 1; i++) {
            Point src = points[i];
            Point dest = points[i + 1];

            Primitives::drawLine(surface, src.x, src.y, dest.x, dest.y, color);
        }
    }

    void setPoints(const std::vector<Point>& newPoints) {
        points = newPoints;
    }

    std::vector<Point>& getPoints() { return points; }

    void setColor(Uint32 newColor) { color = newColor; }
};

#endif
