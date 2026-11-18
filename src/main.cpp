#include "raylib.h"
#include "plane.h"
#include "planet.h"
#include <Window.h>



int main() {
    // Window
    const int screenWidth = 1280; //screen size
    const int screenHeight = 720; //screen size
    InitWindow(screenWidth, screenHeight, "Universe Simulator");

    // 3D Camera
    Camera3D camera = { 0 };
    camera.position = { 5.0f, 5.0f, 5.0f };
    camera.target = { 0.0f, 0.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    // FPS
    int refreshRate = GetMonitorRefreshRate(GetCurrentMonitor());
    int targetFPS = (refreshRate > 1) ? refreshRate : 60; // Use display frequency if available, otherwise default to 60 FPS
    SetTargetFPS(targetFPS); // Set target FPS (maximum)
    
    // Mouse
    DisableCursor();
    bool isCursorLocked = true;
    
    // Plantes
    planet earth(5.972e24, 1.0f, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, WHITE);
            
    while (!WindowShouldClose()) {
        UpdateCamera(&camera, CAMERA_FREE);
        
        
        // Post rendering logic
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreenWindow(screenWidth, screenHeight);
        }

        // Toggle mouse lock with the TAB key
        if (IsKeyPressed(KEY_TAB)) {
            isCursorLocked = !isCursorLocked;
            if (isCursorLocked) {
                DisableCursor();
               
            } else {
                EnableCursor();
           
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawGrid(20, 1.0f);
        earth.Draw();
        
        



        EndMode3D();

        DrawText("Universe Simulator", 20, 20, 25, WHITE);
        DrawFPS(20, 55);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
