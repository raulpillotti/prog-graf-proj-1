#ifndef SHAPE_H
#define SHAPE_H

#include <SDL2/SDL.h>

class Shape {
public:
    virtual ~Shape() {}


    virtual void normalize(float metersX, float metersY, int screenWidth, int screenHeight) = 0;

    virtual void draw(SDL_Surface* surface) = 0;
};

#endif
