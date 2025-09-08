#include "Polygon.h"
#include "Primitives.h"
#include <algorithm>
#include <vector>

void Polygon::setPoints(const std::vector<Point>& newPoints) {
    worldPoints = newPoints;
}

void Polygon::setColor(Uint32 newColor) {
    color = newColor;
}

void Polygon::transform(const Matrix& m) {
    for (auto& point : worldPoints) {
        point = m.transform(point);
    }
}

void Polygon::normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
    normalizedPoints.clear();
    for (const auto& worldPoint : worldPoints) {
        Point screenPoint = worldPoint;
        screenPoint.normalize(metersX, metersY, screenWidth, screenHeight);
        normalizedPoints.push_back(screenPoint);
    }
}

void Polygon::draw(SDL_Surface* surface) {
    if (normalizedPoints.size() < 3 || !surface) return;


    int minY = normalizedPoints[0].screen_y;
    int maxY = normalizedPoints[0].screen_y;

    for (const auto& p : normalizedPoints) {
        minY = std::min(minY, p.screen_y);
        maxY = std::max(maxY, p.screen_y);
    }

    for (int y = minY; y <= maxY; ++y) {
        std::vector<int> nodeX;
        size_t j = normalizedPoints.size() - 1;
        for (size_t i = 0; i < normalizedPoints.size(); ++i) {
            if ((normalizedPoints[i].screen_y < y && normalizedPoints[j].screen_y >= y) ||
                (normalizedPoints[j].screen_y < y && normalizedPoints[i].screen_y >= y)) {
                
                float x = normalizedPoints[i].screen_x +
                          (float)(y - normalizedPoints[i].screen_y) * (normalizedPoints[j].screen_x - normalizedPoints[i].screen_x) /
                          (float)(normalizedPoints[j].screen_y - normalizedPoints[i].screen_y);
                nodeX.push_back(static_cast<int>(x));
            }
            j = i;
        }
        std::sort(nodeX.begin(), nodeX.end());
        for (size_t i = 0; i < nodeX.size(); i += 2) {
            if (i + 1 < nodeX.size()) {
                Primitives::drawLine(surface, nodeX[i], y, nodeX[i+1], y, color);
            }
        }
    }
}