#include "raylib.h"
#include "rlgl.h"
#include "../include/windows/plane.h"
#include "../include/Planets/planet.h"
#include "../include/windows/Window.h"
#include <array>
#include <algorithm>
#include <cmath>

static constexpr int MAX_MASS_POINTS = 8;

static float MassToVisualStrength(double mass)
{
    if (mass <= 0.0) return 0.0f;
    double exponent = std::log10(mass);
    return static_cast<float>(std::clamp((exponent - 20.0) * 0.12, 0.12, 1.35));
}

int main() {
    // Window
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Universe Simulator");

    // 3D Camera
    Camera3D camera{};
    camera.position = { 5.0f, 5.0f, 5.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // FPS
    int refreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    int targetFPS = (refreshRate > 1) ? refreshRate : 60;
    SetTargetFPS(targetFPS);
    
    // Mouse
    DisableCursor();
    bool isCursorLocked = true;
    
    // Planets
    const std::array<Element, 10> earthComposition{
        Element::Oxygen, Element::Nitrogen, Element::Carbon, Element::Hydrogen,
        Element::Helium, Element::Iron, Element::Silicon, Element::Magnesium,
        Element::Sulfur, Element::Calcium
    };
    planet earth{
        5.972e24, 1.0f,
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.65f},
        earthComposition
    };
    planet moon{
        7.342e22, 0.27f,
        {2.5f, 0.0f, 0.0f},
        {0.0f, 0.0f, -0.20f},
        earthComposition
    };
    
    // 1. Load shader for plane
    Shader bezierShader = Load_plane_shader();
    
    // 2. Create a subdivided 3D plane mesh so it has enough vertices to bend along the curve
    Mesh planeMesh = GenMeshPlane(12.0f, 12.0f, 64, 64);
    Model planeModel = LoadModelFromMesh(planeMesh);

    // 3. Attach your custom Bezier shader to the model's material
    planeModel.materials[0].shader = bezierShader;

    int massCountLocation = GetShaderLocation(bezierShader, "massCount");
    int massPointsLocation = GetShaderLocation(bezierShader, "massPoints");
    
    while (!WindowShouldClose()) {
        // BACKEND UPDATES
        UpdateCamera(&camera, CAMERA_FREE);
        float deltaTime = GetFrameTime();
        earth.Update(deltaTime);
        moon.Update(deltaTime);
        
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreenWindow(screenWidth, screenHeight);
        }

        if (IsKeyPressed(KEY_TAB)) {
            isCursorLocked = !isCursorLocked;
            if (isCursorLocked) {
                DisableCursor();
            } else {
                EnableCursor();
            }
        }

        // FRONTEND DRAWING
        BeginDrawing();
        ClearBackground(BLACK);
        
        BeginMode3D(camera);

            earth.Draw();
            moon.Draw();

            std::array<Vector4, MAX_MASS_POINTS> massPoints{};
            massPoints[0] = {
                earth.attributes.position.x,
                earth.attributes.position.y,
                earth.attributes.position.z,
                MassToVisualStrength(earth.attributes.mass)
            };
            massPoints[1] = {
                moon.attributes.position.x,
                moon.attributes.position.y,
                moon.attributes.position.z,
                MassToVisualStrength(moon.attributes.mass)
            };

            int massCount = 2;
            SetShaderValue(bezierShader, massCountLocation, &massCount, SHADER_UNIFORM_INT);
            SetShaderValueV(
                bezierShader,
                massPointsLocation,
                massPoints.data(),
                SHADER_UNIFORM_VEC4,
                massCount
            );

            rlEnableWireMode();

            DrawModel(planeModel, (Vector3){0, 0, 0}, 1.0f, WHITE);

            rlDisableWireMode();
            
            EndMode3D();

            DrawText("Universe Simulator", 20, 20, 25, WHITE);
            DrawFPS(20, 55);

            EndDrawing();
    }

    // Cleanup resources
    UnloadModel(planeModel);
    UnloadShader(bezierShader);

    CloseWindow();

    return 0;
}