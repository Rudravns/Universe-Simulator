# Universe Simulator

A 3D universe simulator built with **C++20** and **Raylib**.

The project renders celestial bodies in a 3D scene and uses a custom shader to create a curved spacetime-style plane around them.

## Features

- 3D rendering with Raylib
- Free-look 3D camera
- Earth and Moon models with element-based colors
- Visual scaling from physical radii into scene units
- Circular orbital velocity initialization when adding a body around a named parent
- Custom vertex shader for a warped spacetime plane
- Basic per-frame position updates
- Fullscreen toggle (`F11`)
- Mouse cursor lock toggle (`TAB`)
- Planet focus with a rendered-radius camera boundary and planet following

## Tech Stack

- **Language:** C++20
- **Graphics Library:** Raylib
- **Compiler:** GCC (MinGW-w64)
- **Build System:** PowerShell
- **Shader:** GLSL 330

## Project Structure

```text
Universe-Simulator/
├── include/
│   ├── Planets/
│   │   ├── attr.h
│   │   ├── celestial.h
│   │   └── planet.h
│   └── windows/
│       ├── Camera.h
│       ├── Window.h
│       └── plane.h
├── src/
│   ├── main.cpp
│   ├── Planets/
│   │   ├── attr.cpp
│   │   ├── celestial.cpp
│   │   └── planet.cpp
│   └── windows/
│       ├── Camera.cpp
│       ├── plane.cpp
│       └── window.cpp
├── Shaders/
│   └── Vertex/
│       └── 2d_plane.glsl
├── build.ps1
└── README.md
```

## Controls

| Key | Action |
|---|---|
| WASD | Move the camera |
| Space / Left Ctrl | Move the camera up / down |
| Left Shift | Sprint |
| Mouse | Camera rotation |
| Up / Down | Focus the next / previous planet |
| X | Clear planet focus |
| TAB | Toggle mouse lock |
| F11 | Toggle fullscreen |
| ESC | Exit simulation |

## Requirements

- Windows
- GCC / MinGW-w64
- C++20 support
- Raylib for MinGW with both `include/raylib.h` and `lib/libraylib.a`

## Building and Running

1. Install GCC / MinGW-w64.
2. Download and extract a MinGW-compatible Raylib build.
3. Set `RAYLIB_ROOT` or pass `-RaylibRoot` if Raylib is outside a standard location.
4. Open the project directory in PowerShell.
5. Run:

```powershell
.\build.ps1
```

Set `RAYLIB_ROOT` or pass `-RaylibRoot` when Raylib is outside a standard location:

```powershell
.\build.ps1 -RaylibRoot "C:\path\to\raylib-6.0_win64_mingw-w64"
```

Use `-Compiler C:\path\to\g++.exe` if MinGW-w64 is not on `PATH`, or `-NoRun` to compile without starting the simulator.

The script compiles all C++ sources into `Universe-Simulator.exe` and runs it if compilation succeeds.

## Future Goals

Future development may add N-body gravity, orbital mechanics, multiple interacting celestial bodies, and more accurate physical-to-visual scaling.

## Code Documentation

The C++ interfaces use Doxygen-style documentation for classes, functions, constants, and parameters. Generated API documentation can be created with Doxygen if it is installed:

```powershell
doxygen
```

Key documented areas include:

- `planet`: construction, motion, acceleration, thrust, rendering, and derived shader properties.
- `Celestial_Objects`: named planet storage, lookup, physics updates, and drawing.
- Physics helpers: gravitational acceleration and distance calculations.
- Window helpers: fullscreen toggling and window resizing.
- Plane helpers: loading the spacetime vertex shader.
- Focus controls: Up/Down cycle planets; X clears focus. The camera follows the focused planet and is eased back inside its rendered-radius boundary.

When editing a public function or parameter, update both its declaration comment and, when behavior differs, its implementation comment.

---

**Status:** In Development
