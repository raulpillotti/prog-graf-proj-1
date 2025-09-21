#ifndef TREE_H
#define TREE_H

#include "Polygon.h"
#include "Shape.h"
#include <vector>

class Tree : public Shape {
private:
    Polygon trunk;
    Polygon leaves;

public:
    Tree();

    void build(float x, float y, float width, float height, Uint32 trunkColor, Uint32 leavesColor, float inclination);
    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) override;
    void draw(SDL_Surface* surface) override;
};

#endif
