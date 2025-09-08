#ifndef FENCE_H
#define FENCE_H

#include "Polygon.h"
#include "Shape.h"
#include <vector>

class Fence : public Shape {
private:
    Polygon post; 

public:
    Fence();

    void build(float x, float y, float width, float height, Uint32 color, float inclination);

    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) override;

    void draw(SDL_Surface* surface) override;
};

#endif

