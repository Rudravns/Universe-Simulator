#include "raylib.h"
#include <cmath>


double const G = 6.6743;


Vector3 accelerationDueToGravity(Vector4 planet1, Vector4 planet2);
Vector3 distanceBetweenPlanets(Vector4 planet1, Vector4 planet2);