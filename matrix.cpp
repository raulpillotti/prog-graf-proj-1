#include "matrix.h"
#include <math.h>
#include <stdio.h>

Point getPoint(int x, int y)
{
    Point p;
    p.x = x;
    p.y = y;
    return p;
}

double toRadians(double degrees)
{
    return degrees * 0.017453293;
}

void translatePolygon(Point poly[], int numPoints, double tx, double ty)
{
    for(int i = 0; i < numPoints; i++) {
        poly[i].x = poly[i].x + tx;
        poly[i].y = poly[i].y + ty;
    }
}

void scalePolygon(Point poly[], int numPoints, double sx, double sy)
{
    for(int i = 0; i < numPoints; i++) {
        poly[i].x = poly[i].x * sx;
        poly[i].y = poly[i].y * sy;
    }
}

void scalePolygon(Point poly[], int numPoints, double sx, double sy, int px, int py)
{
    translatePolygon(poly, numPoints, -px, -py);
    scalePolygon(poly, numPoints, sx, sy);
    translatePolygon(poly, numPoints, px, py);
}

void rotatePolygon(Point poly[], int numPoints, double angle)
{
    double x, y, radians, cosTheta, sinTheta;

    radians  = toRadians(angle);
    cosTheta = cos(radians);
    sinTheta = sin(radians);

    for(int i = 0; i < numPoints; i++) {
        x = poly[i].x;
        y = poly[i].y;
        poly[i].x = x * cosTheta - y * sinTheta;
        poly[i].y = x * sinTheta + y * cosTheta;
    }
}

void rotatePolygon(Point poly[], int numPoints, double angle, int px, int py)
{
    //translatePolygon(poly, numPoints, -px, -py);
    rotatePolygon(poly, numPoints, angle);
    //translatePolygon(poly, numPoints, px, py);
}

void shearPolygon(Point poly[], int numPoints, double c, double d)
{
    double x, y;
    for(int i = 0; i < numPoints; i++) {
        x = poly[i].x;
        y = poly[i].y;
        poly[i].x = x + c * y;
        poly[i].y = d * x + y;
    }
}

void copyPolygon(Point polyFrom[], int numPoints, Point polyTo[])
{
    for(int i = 0; i < numPoints; i++) {
        polyTo[i].x = polyFrom[i].x;
        polyTo[i].y = polyFrom[i].y;
    }
}
