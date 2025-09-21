#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point {
public:
    // metros (SRU)
    float world_x;
    float world_y;

    // pixels (SRD)
    int screen_x;
    int screen_y;

public:
    Point(float wx = 0.0f, float wy = 0.0f)
        : world_x(wx), world_y(wy), screen_x(0), screen_y(0) {}

    void normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
        if (metersX <= 0.0f || metersY <= 0.0f) return;

        screen_x = static_cast<int>((world_x / metersX) * screenWidth);
        screen_y = static_cast<int>(((metersY - world_y) / metersY) * screenHeight);
        this->print();
    }

    void print() const {
        std::cout << "Point World: (" << world_x << ", " << world_y << ") "
                  << "-> Screen: (" << screen_x << ", " << screen_y << ")" << std::endl;
    }
};

#endif
