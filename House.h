#ifndef HOUSE_H
#define HOUSE_H

#include "Polygon.h"

class House : public Shape {
private:
    Polygon wall;
    Polygon roof;
    Polygon door;

public:
    House() = default;

    void draw(SDL_Surface* surface) override {
        wall.draw(surface);
        roof.draw(surface);
        door.draw(surface);
    }

    Polygon& getWall() { return wall; }
    Polygon& getRoof() { return roof; }
    Polygon& getDoor() { return door; }
};

#endif
