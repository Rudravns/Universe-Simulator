# Universe Simulator

A 3D universe simulation built with **C++20** and **Raylib 6.0**.

The goal of this project is to create an interactive 3D environment that simulates celestial bodies and gravitational interactions.

## Features

- 3D rendering with Raylib
- Free 3D camera movement
- Fullscreen toggle (`F11`)
- Mouse cursor lock toggle (`TAB`)
- Celestial bodies (planets, moons, stars)
- Newtonian gravitational simulation
- N-body physics
- Orbital mechanics
- Solar system simulation

## Tech Stack

- **Language:** C++20
- **Graphics Library:** Raylib 6.0
- **Compiler:** GCC (MinGW-w64)
- **Build System:** PowerShell

## Project Structure

```text
UniverseSimulator/
│
├── include/
│   └── Window.h
│
├── src/
│   ├── main.cpp
|   ├── Planets
|   |   ├── planet.cpp
|   |   └── attr.cpp
│   └── windows
|       └── window.cpp
│
├── build.ps1
├── .gitignore
└── README.md
```

## Controls

| Key | Action |
|---|---|
| WASD | Camera movement |
| Mouse | Camera rotation |
| TAB | Toggle mouse lock |
| F11 | Toggle fullscreen |
| ESC | Exit simulation |

## Requirements

- Windows
- GCC / MinGW-w64
- C++20 support
- Raylib 6.0

## Building and Running

1. Install GCC / MinGW-w64.
2. Download and extract Raylib 6.0.
3. Update the Raylib paths in `build.ps1` if necessary.
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

The script compiles all C++ sources into `main.exe` and runs it if compilation succeeds.

## Future Goals

The long-term goal is to build a 3D universe simulator featuring realistic gravitational interactions, orbital mechanics, and multiple interacting celestial bodies.

---

**Status:** In Development