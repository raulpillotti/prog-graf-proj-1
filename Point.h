#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
public:
    float x;
    float y;

public:
    Point(float x, float y)
        : x(x), y(y) {}

    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
        if (metersX > 0.0f) x = (x / metersX) * screenWidth;
        if (metersY > 0.0f) y = (y / metersY) * screenHeight;
    }

    void print() const {
        std::cout << "Point: (" << x << ", " << y << ")" << std::endl;
    }
};

#endif
