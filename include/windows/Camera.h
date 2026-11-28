#pragma once

#include <raylib.h>
#include <raymath.h>
#include <algorithm>
#include <cmath>

struct PlayerCamera
{
private:
    Camera3D cam{};
    float yaw = 0.0f;   // Horizontal rotation (radians)
    float pitch = 0.0f; // Vertical rotation (radians)
    float walkSpeed = 10.0f;
    float sprintSpeed = 22.0f;
    float accel = 12.0f; // Higher = snappier, lower = floatier
    Vector3 velocity = {0, 0, 0};
    const Vector3* followedFocusPosition = nullptr;
    Vector3 previousFocusPosition = {0, 0, 0};

public:
    float sensitivity = 0.003f;

    PlayerCamera();
    void Update(float deltatime, const Vector3* focusPosition, float maxFocusDistance);
    const Camera3D &GetRaylibCamera() const;
};

