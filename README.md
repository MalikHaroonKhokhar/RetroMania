# Forge2D Engine

[![CI Build and Test](https://github.com/USER/REPO/actions/workflows/test.yml/badge.svg)](https://github.com/USER/REPO/actions/workflows/test.yml)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
![C++](https://img.shields.io/badge/C++-20-blue.svg)

All code in this repository was written entirely by Jules. This repository serves as a test/demonstration of Jules' AI coding capabilities.

## Architecture Overview
Forge2D follows a Data-Oriented Design approach through its custom Entity-Component-System (ECS). The engine architecture is split into independent sub-modules that communicate via components and an Event System:

*   **Core:** Math (`Vec2`), Utilities (`Timer`, `Random`, `Log`, `Profiler`), Windowing & Input abstraction.
*   **ECS:** Lightweight Entity Component System with distinct generic types `Registry` and `ComponentArray`.
*   **Rendering:** Based on SDL2, SDL_image, and SDL_ttf. Exposes an abstracted 2D API (`DrawRect`, `DrawTexture`).
*   **Physics:** A simple custom integration model resolving Axis-Aligned Bounding Box (AABB) collisions.
*   **Audio:** Abstraction over SDL_mixer to play and manage BGM and SFX via AudioSource components.
*   **Scene:** Provides a JSON serializer/deserializer to manage states and components for scene loading.
*   **Application:** Standard layered architecture to inject specific game scripts and behavior.

## Features
- Core Engine Architecture: Event system, fixed timestep game loop, resource manager.
- Rendering: SDL2 based 2D sprite rendering, textures, and fonts.
- Physics & Collision: AABB and circle collisions, basic rigidbody physics.
- ECS (Entity Component System): Lightweight and robust.
- Audio: Sound effects and music via SDL_mixer.
- Scene Management: Serialization to JSON and transitions.

## How to Build
This project uses CMake and FetchContent to pull its dependencies automatically.
### Prerequisites:
Make sure you have CMake and a C++20 compatible compiler installed, along with basic build dependencies for SDL2 on your system (like ALSA, X11 libraries).
```bash
sudo apt-get update
sudo apt-get install -y cmake g++ libasound2-dev libx11-dev libxext-dev libxrender-dev libxrandr-dev libxi-dev libglu1-mesa-dev xorg-dev libsdl2-image-dev
```

### Build Instructions:
```bash
cmake -B build
cmake --build build -j$(nproc)
```

## How to Run the Demo
Once built, you can run the demo executable:
```bash
./build/Demo
```

## How to Run the Tests
This engine employs a robust test pyramid validating individual units to full E2E boot integration.
```bash
# Run all tests suites
ctest --test-dir build --output-on-failure

# Or run them individually:
./build/UnitTests
./build/IntegrationTests
./build/E2ETests
```

### Test Pyramid
```text
       / \         -> E2E Tests (1)
      /---\        -> Integration Tests (4+)
     /-----\       -> Unit Tests (30+)
    /-------\
```

## Directory Structure
```text
.
├── CMakeLists.txt
├── README.md
├── demo/                   # Demo game code using the engine
├── engine/                 # Engine source files
│   ├── audio/
│   ├── core/
│   ├── ecs/
│   ├── math/
│   ├── physics/
│   ├── rendering/
│   └── scene/
└── tests/                  # Test suites
    ├── e2e/
    ├── integration/
    └── unit/
```

## Dependencies and Credits
- [SDL2](https://github.com/libsdl-org/SDL) (Windowing, Input)
- [SDL2_image](https://github.com/libsdl-org/SDL_image) (Texture Loading)
- [SDL2_mixer](https://github.com/libsdl-org/SDL_mixer) (Audio)
- [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf) (Fonts)
- [nlohmann/json](https://github.com/nlohmann/json) (Serialization)
- [GoogleTest](https://github.com/google/googletest) (Testing framework)

## Built by Jules
All code in this engine was meticulously generated and engineered by Jules.
