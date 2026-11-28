#include "raylib.h"
#include "rlgl.h"
#include "../include/windows/plane.h"
#include "../include/Planets/planet.h"
#include "../include/Physics/Planets/planethandler.h"
#include "../include/windows/Window.h"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cmath>
#include "Planets/celestial.h"


static constexpr float SIMULATION_TIME_SCALE = 10.0f;
static constexpr float MAX_SIMULATION_STEP = 1.0f / 60.0f;

/**
 * @brief Processes keyboard controls for Earth movement and stopping.
 * @param earth Planet controlled by the arrow and stop keys.
 * @param realDeltaTime Real elapsed frame time in seconds.
 */
void ArrowKeys(planet& earth, float realDeltaTime) {

    if (IsKeyDown(KEY_UP)) {
        earth.PushForward(10 * realDeltaTime / SIMULATION_TIME_SCALE);
    }
    if (IsKeyDown(KEY_DOWN)) {
        earth.PushForward(-10 * realDeltaTime / SIMULATION_TIME_SCALE);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        earth.updateAngle({0, -90 * realDeltaTime, 0});
    }

    if (IsKeyDown(KEY_LEFT)) {
        earth.updateAngle({0, 90 * realDeltaTime, 0});
    }

    if (IsKeyPressed(KEY_X)) {
        earth.Stop();
    }
}

/// @brief Initializes the scene, runs the render/update loop, and cleans up resources.
/// @return Process exit code.
int main() {

    
    // Window
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "Universe Simulator");

    // 3D Camera
    Camera3D camera{};
    camera.position = { 0.0f, 55.0f, 70.0f };
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
    
    // celestial objects
    Celestial_Objects celestialObjects;
    
    // Creating Earth Color
    const std::array<Element, 10> earthComposition{
        Element::Oxygen, Element::Nitrogen, Element::Carbon, Element::Hydrogen,
        Element::Helium, Element::Iron, Element::Silicon, Element::Magnesium,
        Element::Sulfur, Element::Calcium
    };
    
    constexpr double earthMass = 5.972e24;
    constexpr double moonMass = 7.342e22;
    constexpr double sunMass = 1.9891e30;
    constexpr float earthOrbitDistance = 30.0f;
    constexpr float moonOrbitDistance = 0.13f;

    celestialObjects.add("Sun",
    planet(sunMass, 695700, YELLOW)
    );

    // Creating Earth
    celestialObjects.addOrbiting("Earth", "Sun",
        planet(earthMass, 6371.0, earthComposition),
        {earthOrbitDistance, 0.0f, 0.0f}
    );
    
    // Creating Moon
    celestialObjects.addOrbiting("Moon", "Earth",
        planet(moonMass, 1737.0, GRAY),
        {moonOrbitDistance, 0.0f, 0.0f}
    );

    // 1. Load shader for plane
    Shader bezierShader = Load_plane_shader();

    // 2. Create a subdivided 3D plane mesh so it has enough vertices to bend along the curve
    constexpr float fabricSize = 120.0f;
    constexpr int fabricSubdivisions = 256;
    Mesh planeMesh = GenMeshPlane(
        fabricSize, fabricSize, fabricSubdivisions, fabricSubdivisions);
    Model planeModel = LoadModelFromMesh(planeMesh);

    // 3. Attach custom Bezier shader to the model's material
    planeModel.materials[0].shader = bezierShader;

    // Mass Related Calculations
    int massCountLocation = GetShaderLocation(bezierShader, "massCount");
    int massPointsLocation = GetShaderLocation(bezierShader, "massPoints");
    const int massCount = static_cast<int>(std::min(
        celestialObjects.get_all().size(),
        static_cast<std::size_t>(Celestial_Objects::MAX_MASS_POINTS)));
    
    // Should window close? That's up to you!
    while (!WindowShouldClose()) {
        // BACKEND UPDATES
        // Camera
        UpdateCamera(&camera, CAMERA_FREE);
        // DeltaTime
        const float realDeltaTime = GetFrameTime(); //actual dt
        const float deltaTime = realDeltaTime * SIMULATION_TIME_SCALE;

        

        // Key Logic
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



        ArrowKeys(celestialObjects.get("Earth"), realDeltaTime);

        //limiting after how many time physics updates
        const int physicsSteps = std::max(
            1, static_cast<int>(std::ceil(deltaTime / MAX_SIMULATION_STEP)));
        const float physicsStep = deltaTime / physicsSteps;
        for (int step = 0; step < physicsSteps; ++step) {
            celestialObjects.update(physicsStep);
        }
       
        SetShaderValue(bezierShader, massCountLocation, &massCount, SHADER_UNIFORM_INT);
        SetShaderValueV(
            bezierShader,
            massPointsLocation,
            celestialObjects.massPoints().data(),
            SHADER_UNIFORM_VEC4,
            massCount);

        // FRONTEND DRAWING
        BeginDrawing();
        ClearBackground(BLACK);
        
        BeginMode3D(camera);

        // Keep the warped grid and planet surfaces visible from either side.
        rlDisableBackfaceCulling();
        rlDisableDepthMask();
        rlEnableWireMode();
        
        DrawModel(planeModel, (Vector3){0, 0, 0}, 1.0f, WHITE);

        rlDisableWireMode();
        rlEnableDepthMask();

        celestialObjects.draw();

        rlEnableBackfaceCulling();

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

 