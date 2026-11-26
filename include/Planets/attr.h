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

    //structure 
    double mass;
    double radius;
    
    // compostion
    std::array<Element, 10> composition;
    
    // space position
    Vector3 position;
    Vector3 velocity = {0.0f, 0.0f, 0.0f};
    Vector3 acceleration = {0.0f, 0.0f, 0.0f};
    Vector3 Direction = {0.0f, 0.0f, 0.0f};
    
    
};

#endif // CELESTIAL_ATTR_H