#include "raylib.h"
#include "windows/plane.h"

/*
    @brief Computes a point on a cubic Bezier curve in 3D space given four control points and a parameter t.
    @param p0 The first control point (start point).
    @param p1 The second control point.
    @param p2 The third control point.
    @param p3 The fourth control point (end point).
    @param t The parameter (0 <= t <= 1) that determines the position on the curve.


    
    Formula: B(t) = (1 - t)^3 * P0 + 3(1 - t)^2 * t * P1 + 3(1 - t) * t^2 * P2 + t^3 * P3

    u = (1 - t) 
    
    a = u^3

    b = 3(u)^2 * t

    c = 3u * t^2
    
    d = t^3

    Formula in terms of a, b, c, d: a * P0 + b * P1 + c * P2 + d * P3

Vector3 Bezier3d(Vector3 p0, Vector3 p1,Vector3 p2,Vector3 p3, float t) 
{
   
    float u = 1.0f - t;
    float a = u*u*u;
    float b = 3.0f * u * u * t;
    float c = 3.0f * u * t * t;
    float d = t * t * t;


    return {
        a*p0.x + b*p1.x + c*p2.x + d*p3.x,
        a*p0.y + b*p1.y + c*p2.y + d*p3.y,
        a*p0.z + b*p1.z + c*p2.z + d*p3.z
    };
}

*/

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