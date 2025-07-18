#include "math.h"

#include <math.h>

Vector
vectorTranslate(Vector a, Vector b) {
    return (Vector) {
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

Vector
vectorScale(Vector a, double b) {
    return (Vector) {
        .x = a.x * b,
        .y = a.y * b,
    };
}

double
vectorMagnitude(Vector a) {
    return sqrt(a.x * a.x + a.y * a.y);
}

Vector
vectorNormalize(Vector a) {
    double m = vectorMagnitude(a);
    return (Vector) {
        .x = a.x / m,
        .y = a.y / m,
    };
}

Vector
vectorRotate(Vector a, double b) {
    return (Vector) {
        .x = a.x * cos(b) - a.y * sin(b),
        .y = a.x * sin(b) + a.y * cos(b)
    };
}
