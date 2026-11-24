// Gemini code this for me

#include "raylib.h"
#include "Physics/Planets/planethandler.h"


/*
    Vector4: x position, y position, z position, mass

    Formula: g = (G * m1 * m2) / r
*/
Vector3 accelerationDueToGravity(Vector4 planet1, Vector4 planet2) {
    Vector3 r = distanceBetweenPlanets(planet1, planet2);

    Vector3 g = {0};
    g.x = G * planet1.w * planet2.w / r.x;
    g.y = G * planet1.w * planet2.w / r.y;
    g.z = G * planet1.w * planet2.w / r.z;

    return g;
}

// Vector4: x position, y position, z position, mass
Vector3 distanceBetweenPlanets(Vector4 planet1, Vector4 planet2) {
    double x = std::pow(planet2.x - planet1.x, 2);
    double y = std::pow(planet2.y - planet1.y, 2);
    double z = std::pow(planet2.z - planet1.z, 2);

    Vector3 d = {0};
    d.x = std::sqrt(x);
    d.y = std::sqrt(y);
    d.z = std::sqrt(z);

    return d;
}

//void updatePlanet(Vector4 planet)