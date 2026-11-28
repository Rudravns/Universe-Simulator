#include "windows/Camera.h"


PlayerCamera::PlayerCamera()
{
    cam.position = {0.0f, 55.0f, 70.0f};
    cam.target = {0.0f, 0.0f, 0.0f}; //where u r looking at
    cam.up = {0.0f, 1.0f, 0.0f}; // the up vector
    cam.fovy = 45.0f; 
    cam.projection = CAMERA_PERSPECTIVE;
}

/// @brief Updates the custom build camera
void PlayerCamera::Update(float deltatime, const Vector3* focusPosition,
                          float maxFocusDistance)
{
    if (focusPosition == nullptr)
    {
        followedFocusPosition = nullptr;
    }
    else
    {
        if (focusPosition == followedFocusPosition)
        {
            const Vector3 focusMovement =
                Vector3Subtract(*focusPosition, previousFocusPosition);
            cam.position = Vector3Add(cam.position, focusMovement);
        }

        followedFocusPosition = focusPosition;
        previousFocusPosition = *focusPosition;
    }

    // --- 1. Mouse Rotation ---
    Vector2 mouseDelta = GetMouseDelta();
    yaw -= mouseDelta.x * sensitivity;
    pitch -= mouseDelta.y * sensitivity;

    const float maxPitch = 89.0f * DEG2RAD;
    pitch = std::clamp(pitch, -maxPitch, maxPitch);

    Vector3 lookDir;
    lookDir.x = std::cos(pitch) * std::sin(yaw);
    lookDir.y = std::sin(pitch);
    lookDir.z = std::cos(pitch) * std::cos(yaw);

    // --- 2. Direction Vectors ---
    Vector3 forward = Vector3Normalize({lookDir.x, 0.0f, lookDir.z});
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, cam.up));

    // --- 3. Input Direction ---
    Vector3 inputDir = {0.0f, 0.0f, 0.0f};

    if (IsKeyDown(KEY_W))
        inputDir = Vector3Add(inputDir, forward);
    if (IsKeyDown(KEY_S))
        inputDir = Vector3Subtract(inputDir, forward);
    if (IsKeyDown(KEY_A))
        inputDir = Vector3Subtract(inputDir, right);
    if (IsKeyDown(KEY_D))
        inputDir = Vector3Add(inputDir, right);
    if (IsKeyDown(KEY_SPACE))
        inputDir.y += 1.0f;
    if (IsKeyDown(KEY_LEFT_CONTROL))
        inputDir.y -= 1.0f;

    if (Vector3Length(inputDir) > 0.0f)
    {
        inputDir = Vector3Normalize(inputDir);
    }

    // --- 4. Sprint & Smooth Acceleration ---
    float currentSpeed = IsKeyDown(KEY_LEFT_SHIFT) ? sprintSpeed : walkSpeed;
    Vector3 targetVelocity = Vector3Scale(inputDir, currentSpeed);

    // Frame-rate independent exponential smoothing
    float blend = 1.0f - std::exp(-accel * deltatime);
    velocity = Vector3Lerp(velocity, targetVelocity, blend);

    // Apply velocity to position
    cam.position = Vector3Add(cam.position, Vector3Scale(velocity, deltatime));

    if (focusPosition != nullptr && maxFocusDistance >= 0.0f)
    {
        const Vector3 offset = Vector3Subtract(cam.position, *focusPosition);
        const float distance = Vector3Length(offset);
        if (distance > maxFocusDistance && distance > 0.0f)
        {
            const Vector3 boundaryPosition = Vector3Add(
                *focusPosition,
                Vector3Scale(offset, maxFocusDistance / distance));
            cam.position = Vector3Lerp(cam.position, boundaryPosition, blend);
        }
    }

    cam.target = Vector3Add(cam.position, lookDir);
}

/// @brief Returns the Raylib cam
const Camera3D &PlayerCamera::GetRaylibCamera() const
{
    return cam;
}
