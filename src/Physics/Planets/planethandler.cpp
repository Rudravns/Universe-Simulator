#include "raylib.h"
#include "raymath.h"
#include "Physics/Planets/planethandler.h"
#include <cmath>


/*
    Vector4: x position, y position, z position, mass
*/
Vector3 accelerationDueToGravity(Vector4 planet1, Vector4 planet2) {
    Vector3 direction = {
        planet2.x - planet1.x,
        planet2.y - planet1.y,
        planet2.z - planet1.z
    };
    const double distanceSquared =
        static_cast<double>(direction.x) * direction.x +
        static_cast<double>(direction.y) * direction.y +
        static_cast<double>(direction.z) * direction.z;
    if (distanceSquared == 0.0) {
        return {0.0f, 0.0f, 0.0f};
    }

    const double distance = std::sqrt(distanceSquared);
    const double sceneDistanceSquared =
        distanceSquared * METERS_PER_SCENE_UNIT * METERS_PER_SCENE_UNIT;
    const double accelerationScale =
        G * planet2.w /
        (sceneDistanceSquared * METERS_PER_SCENE_UNIT);
    return {
        static_cast<float>(accelerationScale * direction.x / distance),
        static_cast<float>(accelerationScale * direction.y / distance),
        static_cast<float>(accelerationScale * direction.z / distance)
    };
}

// Vector4: x position, y position, z position, mass
Vector3 distanceBetweenPlanets(Vector4 planet1, Vector4 planet2) {
    double x = std::pow(planet2.x - planet1.x, 2);
    double y = std::pow(planet2.y - planet1.y, 2);
    double z = std::pow(planet2.z - planet1.z, 2);

    Vector3 d = {0.0f, 0.0f, 0.0f};
    d.x = std::sqrt(x);
    d.y = std::sqrt(y);
    d.z = std::sqrt(z);

    return d;
}

