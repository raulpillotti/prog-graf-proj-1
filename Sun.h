#ifndef SUN_H
#define SUN_H

#include "Polygon.h"

class Sun : public Shape {
private:
    Polygon body; // o sol é representado por um polígono

public:
    Sun() = default;

    void draw(SDL_Surface* surface) override {
        body.draw(surface);
    }

    Polygon& getBody() { return body; }
};

#endif
