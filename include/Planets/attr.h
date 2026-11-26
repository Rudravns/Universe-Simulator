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

/**
 * @brief Physical and kinematic attributes shared by celestial bodies.
 */
struct celest_attr {
    public:

    /// @brief Mass in kilograms.
    double mass;

    /// @brief Physical radius in kilometers.
    double radius;
    
    /// @brief Element composition used for color generation.
    std::array<Element, 10> composition;
    
    /// @brief Position in scene units.
    Vector3 position;

    /// @brief Linear velocity in scene units per second.
    Vector3 velocity = {0.0f, 0.0f, 0.0f};

    /// @brief Acceleration applied during physics updates.
    Vector3 acceleration = {0.0f, 0.0f, 0.0f};

    /// @brief Facing Euler angles in degrees.
    Vector3 Direction = {0.0f, 0.0f, 0.0f};
    
    
};

#endif // CELESTIAL_ATTR_H