#include "raylib.h"
#include "rlgl.h"
#include "../include/windows/plane.h"
#include "../include/Planets/planet.h"
#include "../include/windows/Window.h"
#include <array>
#include "Planets/celestial.h"

static constexpr int MAX_MASS_POINTS = 8;
void ArrowKeys(planet& earth){
    if (IsKeyDown(KEY_UP)) {
        earth.PushForward(10 * GetFrameTime() * 60);
    }
    if (IsKeyDown(KEY_DOWN)) {
      earth.PushForward(-10 * GetFrameTime() * 60);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        earth.updateAngle({0, -5 * GetFrameTime() * 60, 0});
    }

    if (IsKeyDown(KEY_LEFT)) {
        earth.updateAngle({0, 5 * GetFrameTime() * 60, 0});
    }
}
int main() {
    int age;
    std::cin >> age;
    
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
    
    // celestial objects
    Celestial_Objects celestialObjects;
    
    // Creating Earth Color
    const std::array<Element, 10> earthComposition{
        Element::Oxygen, Element::Nitrogen, Element::Carbon, Element::Hydrogen,
        Element::Helium, Element::Iron, Element::Silicon, Element::Magnesium,
        Element::Sulfur, Element::Calcium
    };
    const std::array<Element, 10> moonComposition{
        Element::Oxygen,
        Element::Silicon,
        Element::Magnesium,
        Element::Iron,
        Element::Calcium,
        Element::Aluminum
    };

    
    // Creating Earth
    celestialObjects.add("Earth", 
        planet(5.972e24, 6371.0, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, earthComposition)
    );
    
    // Creating Moon
    celestialObjects.add("Moon", 
        planet(7.342e22, 1737.0, {2.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, moonComposition)
    );

    // 1. Load shader for plane
    Shader bezierShader = Load_plane_shader();

    // 2. Create a subdivided 3D plane mesh so it has enough vertices to bend along the curve
    Mesh planeMesh = GenMeshPlane(12.0f, 12.0f, std::pow(2, 7), std::pow(2, 7)); //parm are w, l, resx, resy ; res is in bit logic
    Model planeModel = LoadModelFromMesh(planeMesh);

    // 3. Attach your custom Bezier shader to the model's material
    planeModel.materials[0].shader = bezierShader;

    // Mass Related Calculations
    int massCountLocation = GetShaderLocation(bezierShader, "massCount");
    int massPointsLocation = GetShaderLocation(bezierShader, "massPoints");
    std::array<Vector4, MAX_MASS_POINTS> massPoints{};
    
    // Should window close? That's up to you!
    while (!WindowShouldClose()) {
        // BACKEND UPDATES
        // Camera
        UpdateCamera(&camera, CAMERA_FREE);
        // DeltaTime
        float deltaTime = GetFrameTime();
        // Update Planets
        celestialObjects.get("Earth").Update(deltaTime);
        celestialObjects.get("Moon").Update(deltaTime);



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



        // Get position in memory of planet
        planet& earth = celestialObjects.get("Earth");
        planet& moon = celestialObjects.get("Moon");
        massPoints[0] = earth.getMassPoint();
        massPoints[1] = moon.getMassPoint();
        ArrowKeys(earth);
        int massCount = 2;
        SetShaderValue(bezierShader, massCountLocation, &massCount, SHADER_UNIFORM_INT);
        SetShaderValueV(
            bezierShader,
            massPointsLocation,
            massPoints.data(),
            SHADER_UNIFORM_VEC4,
            massCount
        );

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

        earth.Draw();
        moon.Draw();

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

 