#include "Tree.h"
#include "matrix.h"
#include "Point.h"
#include <vector>

Tree::Tree() {}

void Tree::build(float x, float y, float width, float height, Uint32 trunkColor, Uint32 leavesColor, float inclination) {
    float trunkWidth = width * 0.2f;
    float trunkHeight = height * 0.5f;
    trunk.setPoints({
        Point(x, y),
        Point(x + trunkWidth, y),
        Point(x + trunkWidth, y + trunkHeight),
        Point(x, y + trunkHeight)
    });
    trunk.setColor(trunkColor);

    float leavesHeight = height * 0.5f;
    float leavesWidth = width;
    leaves.setPoints({
        Point(x + (trunkWidth / 2.0f) - (leavesWidth / 2.0f), y + trunkHeight),
        Point(x + (trunkWidth / 2.0f) + (leavesWidth / 2.0f), y + trunkHeight),
        Point(x + (trunkWidth / 2.0f), y + trunkHeight + leavesHeight)
    });
    leaves.setColor(leavesColor);

    if (inclination != 0.0f) {
        Point pivot(x + trunkWidth / 2.0f, y);
        Matrix rotationMatrix = Matrix::rotation(inclination, pivot);
        trunk.transform(rotationMatrix);
        leaves.transform(rotationMatrix);
    }
}

void Tree::normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
    trunk.normalize(metersX, metersY, screenWidth, screenHeight);
    leaves.normalize(metersX, metersY, screenWidth, screenHeight);
}

void Tree::draw(SDL_Surface* surface) {
    leaves.draw(surface);
    trunk.draw(surface);
}
