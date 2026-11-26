#ifndef PLANET_H
#define PLANET_H

#include "raylib.h"
#include "attr.h"
#include <array>
#include "raymath.h"
#include <algorithm>

struct Celestial_Objects;

class planet {
public:
    celest_attr attributes;
    Color color;
    planet(double mass, double radius, Vector3 position, Vector3 velocity,
           const std::array<Element, 10>& composition);


    void Update(float deltaTime);
    void Update_Physics(float deltaTime, Celestial_Objects &system);

    void Push(Vector3 velocity_ip);
    void PushForward(double push);
    void Change_Acceleration(Vector3 dt);
    void ZeroAcceleration();
    void Draw() const; 
    void Stop();
    
    void updateAngle(Vector3 delta_angle);
    
    Vector4 getRaw();
    Color getColorFromElementComposition() const;
    Vector4 getMassPoint();
};

#endif