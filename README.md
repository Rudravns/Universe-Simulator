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
│   └── window.cpp
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

The script compiles the project into `main.exe` and runs it if compilation succeeds.

## Future Goals

The long-term goal is to build a 3D universe simulator featuring realistic gravitational interactions, orbital mechanics, and multiple interacting celestial bodies.

---

**Status:** In Development