#ifndef PLANET_H
#define PLANET_H

#include "raylib.h"
#include "attr.h"
#include <array>

class planet {
public:
    celest_attr attributes;
    Color color;
    planet(double mass, double radius, Vector3 position, Vector3 velocity,
           const std::array<Element, 10>& composition);
    void Update(float deltaTime);
    void Draw() const;
    Color getColorFromElementComposition() const;
};

#endif