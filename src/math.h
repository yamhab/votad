#pragma once

#include <math.h>

#define degToRad(a) ((a) * M_PI / 180.0)

typedef struct Vector {
    double x, y;
} Vector;

Vector
vectorTranslate(Vector a, Vector b);

Vector
vectorScale(Vector a, double b);

double
vectorMagnitude(Vector a);

Vector
vectorNormalize(Vector a);

Vector
vectorRotate(Vector a, double b);
