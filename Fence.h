#ifndef FENCE_H
#define FENCE_H

#include "Polygon.h"
#include "Point.h"
#include <vector>

class Fence : public Shape {
private:
    Polygon post;

public:
    Fence() = default;

    void draw(SDL_Surface* surface) override {
        post.draw(surface);
    }

    Polygon& getPost() { return post; }
};

#endif
