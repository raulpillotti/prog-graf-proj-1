#ifndef HOUSE_H
#define HOUSE_H

#include "Polygon.h"
#include "Shape.h"
#include <vector>

class House : public Shape {
private:
    Polygon wall;
    Polygon roof;
    Polygon door;

public:
    House();

    void build(float x, float y, float width, float height, Uint32 wallColor, Uint32 roofColor, Uint32 doorColor);

    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) override;

    void draw(SDL_Surface* surface) override;
};

#endif

