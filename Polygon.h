#ifndef POLYGON_H
#define POLYGON_H

#include "Shape.h"
#include "Point.h"
#include "matrix.h"
#include <vector>
#include <SDL2/SDL.h>

class Polygon : public Shape {
private:
    std::vector<Point> worldPoints;
    std::vector<Point> normalizedPoints;
    Uint32 color;

public:
    Polygon() : color(0xFFFFFFFF) {}

    void setPoints(const std::vector<Point>& newPoints);
    void setColor(Uint32 newColor);
    void transform(const Matrix& m);
    void normalize(float metersX, float metersY, int screenWidth, int screenHeight);
    void draw(SDL_Surface* surface) override;
};

#endif
