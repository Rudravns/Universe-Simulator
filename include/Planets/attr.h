#ifndef CELESTIAL_ATTR_H
#define CELESTIAL_ATTR_H

#include "raylib.h"
#include <array>

enum class Element {
    Hydrogen,
    Helium,
    Oxygen,
    Carbon,
    Nitrogen,
    Iron,
    Silicon,
    Magnesium,
    Sulfur,
    Calcium,
    Aluminum,
};

struct celest_attr { // Celestial Attributes
    public:
    double mass;
    double radius;
    Vector3 position;
    Vector3 velocity;
    std::array<Element, 10> composition;
};

#endif // CELESTIAL_ATTR_H