#ifndef MATRIX_H
#define MATRIX_H

#include "Point.h" 

class Matrix {
private:
    float m[3][3];

public:
    Matrix();
    Point transform(const Point& p) const;

    Matrix multiply(const Matrix& other) const;


    static Matrix translation(float tx, float ty);

    static Matrix scaling(float sx, float sy);

    static Matrix rotation(float degrees);

    static Matrix rotation(float degrees, const Point& pivot);
};

#endif 

