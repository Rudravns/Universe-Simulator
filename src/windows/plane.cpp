#include "raylib.h"
#include "windows/plane.h"

/// @brief Loads the GLSL vertex shader used by the warped plane model.
Shader Load_plane_shader() {
    Shader fabricShader = LoadShader(
        "Shaders/Vertex/2d_plane.glsl",
        nullptr
    );

    if (fabricShader.id == 0)
    {
        TraceLog(LOG_WARNING, "Spacetime shader could not be loaded.");
    }

    return fabricShader;
}