#pragma once
#include "raylib.h"
#include <cmath>

inline constexpr double G = 6.6743e-11;
inline constexpr double METERS_PER_SCENE_UNIT = 6.0e6; //scene is the pxs

/// @brief Calculates acceleration experienced by planet1 because of planet2.
/// @param planet1 Source planet position in XYZ and mass in W.
/// @param planet2 Gravitating planet position in XYZ and mass in W.
/// @return Acceleration vector in scene units.
Vector3 accelerationDueToGravity(Vector4 planet1, Vector4 planet2);

/// @brief Calculates axis-aligned absolute distance components between planets.
/// @param planet1 First planet position in XYZ.
/// @param planet2 Second planet position in XYZ.
/// @return Distance vector in scene units.
Vector3 distanceBetweenPlanets(Vector4 planet1, Vector4 planet2);