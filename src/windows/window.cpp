#include "windows/Window.h"
#include "raylib.h"

/// @brief Switches monitor-fullscreen state while preserving a windowed fallback size.
void ToggleFullscreenWindow(int windowWidth, int windowHeight) {
    if (!IsWindowFullscreen()) { // Windowed --> Fullscreen
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    } else { // Fullscreen --> Windowed
        ToggleFullscreen();
        SetWindowSize(windowWidth, windowHeight);
    }
}

/// @brief Caps the supplied window dimensions at the current monitor dimensions.
void RescaleWindow(int newWindowWidth, int newWindowHeight) {
    // Only rescale window if it's not fullscreen
    if (!IsWindowFullscreen()) {
        // Get monitor and store it's width and size
        int monitor = GetCurrentMonitor();
        int monitorWidth = GetMonitorWidth(monitor);
        int monitorHeight = GetMonitorHeight(monitor);
        
        // If size of monitor is less than size of window, scale window to monitor size
        int width = (monitorWidth > newWindowWidth) ? newWindowWidth : monitorWidth;
        int height = (monitorHeight > newWindowHeight) ? newWindowHeight : monitorHeight;

        // Set window size
        SetWindowSize(width, height);
    }
}