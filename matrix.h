#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

typedef struct {int x, y;} Point;

Point getPoint(int x, int y);

double toRadians(double degrees);

void translatePolygon(Point poly[], int numPoints, double tx, double ty);

void scalePolygon(Point poly[], int numPoints, double sx, double sy);

void scalePolygon(Point poly[], int numPoints, double sx, double sy, int px, int py);

void rotatePolygon(Point poly[], int numPoints, double angle);

void rotatePolygon(Point poly[], int numPoints, double angle, int px, int py);

void shearPolygon(Point poly[], int numPoints, double c, double d);

void copyPolygon(Point polyFrom[], int numPoints, Point polyTo[]);


#endif // MATRIX_H_INCLUDED
