#ifndef SUN_H
#define SUN_H

#include "Polygon.h"

class Sun : public Shape {
private:
    Polygon body;

public:
    Sun() = default;

    void draw(SDL_Surface* surface) override {
        body.draw(surface);
    }

    Polygon& getBody() { return body; }
};

#endif
