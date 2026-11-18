#ifndef PLANE_H
#define PLANE_H


#include "raylib.h"

/* 
-  create a 3d grid
-   

*/

void DrawGrid(int slices, float spacing);

Vector3 Bezier3d(Vector3 p0, Vector3 p1,Vector3 p2,Vector3 p3, float t);



#endif // WINDOW_H