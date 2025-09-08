#ifndef CIRCLE_FILLED_AA_H
#define CIRCLE_FILLED_AA_H

#include "Shape.h"
#include "Primitives.h"
#include "Point.h" 
#include <SDL2/SDL.h>
#include <cmath>

class CircleFilledAA : public Shape {
private:
    Point center;
    float world_radius;
    Uint32 color;

    int screen_radius;

public:
    CircleFilledAA(Point center = Point(0,0), float radius = 0, Uint32 color = 0);

    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) override;

    void draw(SDL_Surface* surface) override;
};

#endif