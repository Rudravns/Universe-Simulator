#include "raylib.h"
#include "plane.h"

void DrawGrid(int slices, float spacing)
{}

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