#ifndef SHAPE_H
#define SHAPE_H

#include <SDL2/SDL.h>

// Classe abstrata (interface para todas as figuras)
class Shape {
public:
    virtual ~Shape() {}
    virtual void draw(SDL_Surface* surface) = 0; // método puro virtual
};

#endif

