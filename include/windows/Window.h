#ifndef WINDOW_H
#define WINDOW_H

/// @brief Toggles between fullscreen and the supplied windowed dimensions.
/// @param windowWidth Window width restored in windowed mode.
/// @param windowHeight Window height restored in windowed mode.
void ToggleFullscreenWindow(int windowWidth, int windowHeight);

/// @brief Fits a requested window size within the current monitor when windowed.
/// @param newWindowWidth Requested window width.
/// @param newWindowHeight Requested window height.
void RescaleWindow(int newWindowWidth, int newWindowHeight);

#endif
    