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

    /**
     * @brief Constructs a celestial body at the origin with an initial velocity.
     * @param mass Mass of the body in kilograms.
     * @param radius Physical radius of the body in kilometers.
     * @param composition Element composition used to derive the body's color.
     * @param velocity Initial velocity in scene units per second.
     */
    planet(double mass, double radius, const std::array<Element, 10>& composition,
           Vector3 velocity = {0.0f, 0.0f, 0.0f});
    /**
     * @brief Constructs a celestial body at the origin with an initial velocity.
     * @param mass Mass of the body in kilograms.
     * @param radius Physical radius of the body in kilometers.
     * @param color Constant color for the body.
     * @param velocity Initial velocity in scene units per second.
     */
    planet(double mass, double radius, Color color,
           Vector3 velocity = {0.0f, 0.0f, 0.0f});

    /// @brief Updates orientation-based motion without gravitational integration.
    /// @param deltaTime Simulation time elapsed since the previous frame.
    void Update(float deltaTime);

    /// @brief Calculates and integrates gravity from every other body in the system.
    /// @param deltaTime Simulation time elapsed since the previous frame.
    /// @param system Collection containing this planet and the other interacting bodies.
    void Update_Physics(float deltaTime, Celestial_Objects &system);

    /// @brief Adds an instantaneous velocity change to the body.
    /// @param velocity_ip Velocity delta to add, in scene units per second.
    void Push(Vector3 velocity_ip);

    /// @brief Applies thrust along the body's current direction.
    /// @param push Forward or reverse thrust magnitude.
    void PushForward(double push);

    /// @brief Adds a delta to the body's current acceleration.
    /// @param dt Acceleration delta to add.
    void Change_Acceleration(Vector3 dt);

    /// @brief Resets the body's acceleration to zero.
    void ZeroAcceleration();

    /// @brief Draws the body as a sphere in the 3D scene.
    void Draw() const; 

    /// @brief Stops all movement by setting velocity to zero.
    void Stop();
    
    /// @brief Changes the body's facing angle.
    /// @param delta_angle Euler-angle delta to add, in degrees.
    void updateAngle(Vector3 delta_angle);
    
    /// @brief Returns the body's position and mass for gravity calculations.
    /// @return Vector4 containing scene position in XYZ and mass in W.
    Vector4 getRaw();

    /// @brief Derives a display color from the body's element composition.
    /// @return Display color normalized to Raylib's 0-255 channel range.
    Color getColorFromElementComposition() const;

    /// @brief Returns the body's position and clamped visual shader strength.
    /// @return Vector4 containing scene position in XYZ and shader strength in W.
    Vector4 getMassPoint();
};

#endif