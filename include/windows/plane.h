#ifndef PLANE_H
#define PLANE_H


#include "raylib.h"

/// @brief Loads the vertex shader used to warp the 3D spacetime plane.
/// @return Loaded Raylib shader; its ID is zero if loading failed.
Shader Load_plane_shader();

#endif // PLANE_H