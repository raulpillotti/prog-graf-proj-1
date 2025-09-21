#ifndef SUN_H
#define SUN_H

#include "Shape.h"
#include "CircleFilledAA.h"
#include "Point.h"

class Sun : public Shape {
private:
    CircleFilledAA body;

public:
    Sun();

    void build(float x, float y, float width, float height, Uint32 color);
    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) override;
    void draw(SDL_Surface* surface) override;
};

#endif
