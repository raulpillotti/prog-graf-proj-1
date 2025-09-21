#include "matrix.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Matrix::Matrix() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

// Aplica a transformação a um ponto
Point Matrix::transform(const Point& p) const {
    float new_x = m[0][0] * p.world_x + m[0][1] * p.world_y + m[0][2];
    float new_y = m[1][0] * p.world_x + m[1][1] * p.world_y + m[1][2];
    return Point(new_x, new_y);
}

// Multiplica esta matriz por outra
Matrix Matrix::multiply(const Matrix& other) const {
    Matrix result;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            result.m[i][j] = m[i][0] * other.m[0][j] +
                             m[i][1] * other.m[1][j] +
                             m[i][2] * other.m[2][j];
        }
    }
    return result;
}

// Cria uma matriz de translação
Matrix Matrix::translation(float tx, float ty) {
    Matrix t;
    t.m[0][2] = tx;
    t.m[1][2] = ty;
    return t;
}

// Cria uma matriz de escala
Matrix Matrix::scaling(float sx, float sy) {
    Matrix s;
    s.m[0][0] = sx;
    s.m[1][1] = sy;
    return s;
}

// Cria uma matriz de rotação em torno da origem
Matrix Matrix::rotation(float degrees) {
    float rad = degrees * M_PI / 180.0f;
    float cos_a = cos(rad);
    float sin_a = sin(rad);
    Matrix r;
    r.m[0][0] = cos_a;
    r.m[0][1] = -sin_a;
    r.m[1][0] = sin_a;
    r.m[1][1] = cos_a;
    return r;
}

// Cria uma matriz de rotação em torno de um pivô
Matrix Matrix::rotation(float degrees, const Point& pivot) {
    Matrix toOrigin = Matrix::translation(-pivot.world_x, -pivot.world_y);
    Matrix rot = Matrix::rotation(degrees);
    Matrix fromOrigin = Matrix::translation(pivot.world_x, pivot.world_y);

    return fromOrigin.multiply(rot.multiply(toOrigin));
}
