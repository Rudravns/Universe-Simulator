#ifndef PLANET_H
#define PLANET_H

#include "raylib.h"
#include "attr.h" // Or wherever celest_attr is defined

class planet {
public:
    celest_attr attributes;

    planet(double mass, double radius, Vector3 position, Vector3 velocity, Color color);
    void Update(float deltaTime);
    void Draw() const;
};

#endif