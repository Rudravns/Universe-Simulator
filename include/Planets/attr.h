#ifndef CELESTIAL_ATTR_H
#define CELESTIAL_ATTR_H

#include "raylib.h"

struct celest_attr { // Celestial Attributes
    double mass;
    double radius;
    Vector3 position;
    Vector3 velocity;
    Color color;
};

#endif // CELESTIAL_ATTR_H