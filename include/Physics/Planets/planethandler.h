#include "raylib.h"
#include <cmath>

inline constexpr double G = 6.6743e-11;
inline constexpr double METERS_PER_SCENE_UNIT = 6.0e6; //scene is the pxs

Vector3 accelerationDueToGravity(Vector4 planet1, Vector4 planet2);
Vector3 distanceBetweenPlanets(Vector4 planet1, Vector4 planet2);