#ifndef TREE_H
#define TREE_H

#include "Polygon.h"
#include "Point.h"
#include <vector>

class Tree : public Shape {
private:
    Polygon trunk;
    Polygon leaves;

public:
    Tree() = default;

    void draw(SDL_Surface* surface) override {
        trunk.draw(surface);
        leaves.draw(surface);
    }

    Polygon& getTrunk() { return trunk; }
    Polygon& getLeaves() { return leaves; }
};

#endif
