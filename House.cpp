#include "House.h"
#include "Point.h"
#include <vector>

House::House() {}

void House::build(float x, float y, float width, float height,
                  Uint32 wallColor, Uint32 roofColor, Uint32 doorColor,
                  float inclination) {
    // Parede
    float roofHeight = height * 0.5f;
    wall.setPoints({
        Point(x, y),
        Point(x + width, y),
        Point(x + width, (y + height) - roofHeight),
        Point(x, (y + height) - roofHeight)
    });
    wall.setColor(wallColor);

    // Telhado
    float roofOverhang = width * 0.1f;
    roof.setPoints({
        Point(x - roofOverhang, (y + height) - roofHeight),
        Point(x + width + roofOverhang, (y + height) - roofHeight),
        Point(x + width / 2.0f, y + height)
    });
    roof.setColor(roofColor);

    // Porta
    float doorWidth = width * 0.25f;
    float doorHeight = height * 0.6f;
    float doorX = x + (width - doorWidth) / 2.0f;
    door.setPoints({
        Point(doorX, y),
        Point(doorX + doorWidth, y),
        Point(doorX + doorWidth, y + doorHeight),
        Point(doorX, y + doorHeight)
    });
    door.setColor(doorColor);

    // Inclinação
    if (inclination != 0.0f) {
        Point pivot(x + width / 2.0f, y);
        Matrix rotationMatrix = Matrix::rotation(inclination, pivot);

        wall.transform(rotationMatrix);
        roof.transform(rotationMatrix);
        door.transform(rotationMatrix);
    }
}


void House::normalize(float metersX, float metersY, int screenWidth, int screenHeight) {
    wall.normalize(metersX, metersY, screenWidth, screenHeight);
    roof.normalize(metersX, metersY, screenWidth, screenHeight);
    door.normalize(metersX, metersY, screenWidth, screenHeight);
}

void House::draw(SDL_Surface* surface) {
    wall.draw(surface);
    roof.draw(surface);
    door.draw(surface);
}
