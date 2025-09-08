#include "Fence.h"
#include "matrix.h"
#include "Point.h"
#include <vector>

Fence::Fence() {}

void Fence::build(float x, float y, float width, float height, Uint32 color, float inclination) {
    post.setPoints({
        Point(x, y),
        Point(x + width, y),
        Point(x + width, y + height),
        Point(x, y + height)
    });
    post.setColor(color);

    if (inclination != 0.0f) {
        Point pivot(x + width / 2.0f, y);
        Matrix rotationMatrix = Matrix::rotation(inclination, pivot);
        post.transform(rotationMatrix);
    }
}

void Fence::normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
    post.normalize(metersX, metersY, screenWidth, screenHeight);
}

void Fence::draw(SDL_Surface* surface) {
    post.draw(surface);
}