# MyRPG

A 2D RPG built in C++ using a custom game engine developed from scratch.

This project focuses on learning and implementing core game engine
concepts including rendering, scene management, physics, and entity
systems. The goal is to build a modular engine capable of supporting
RPG-style gameplay while deepening understanding of graphics programming
and game architecture.

------------------------------------------------------------------------

## Overview

MyRPG is both:

-   A **2D RPG game**
-   A **custom C++ game engine**

The engine provides core systems such as:

-   Rendering
-   Scene management
-   Game object transforms
-   Physics and collision detection
-   Asset management

The project uses **C++ and SDL-based rendering**, a cross‑platform
library used for graphics, input, and window management in games.

------------------------------------------------------------------------

## Features

### Engine

-   2D rendering pipeline
-   Shared quad mesh rendering
-   Orthographic camera system
-   Scene management
-   Model matrix transformations
-   Basic physics system
-   Game object architecture

### Game

-   Player movement
-   Scene transitions
-   Dialogue and text rendering
-   RPG-style world navigation
-   Interactive objects

------------------------------------------------------------------------

## Technologies Used

-   **C++17**
-   **SDL**
-   **OpenGL**
-   **CMake**
-   **GLM** (math library)

------------------------------------------------------------------------

## Project Structure

    MyRPG/
    ├── engine/        # Core engine systems
    │   ├── rendering
    │   ├── physics
    │   ├── scene
    │   └── core
    │
    ├── game/          # Game-specific logic
    │   ├── scenes
    │   ├── entities
    │   └── gameplay
    │
    ├── assets/        # Textures, sprites, and resources
    │
    └── CMakeLists.txt

------------------------------------------------------------------------

## Building the Project

### Requirements

-   C++17 compatible compiler
-   CMake
-   SDL development libraries
-   OpenGL

### Build Steps

Clone the repository:

``` bash
git clone https://github.com/CameronPrybylski/MyRPG.git
cd MyRPG
```

Create a build directory:

``` bash
mkdir build
cd build
```

Run CMake:

``` bash
cmake ..
```

Compile:

``` bash
make
```

Run the game:

``` bash
./MyRPG
```

------------------------------------------------------------------------

## Learning Goals

This project was created to explore:

-   Game engine architecture
-   Graphics programming fundamentals
-   Transform systems and scene graphs
-   Real-time rendering
-   Physics simulation
-   Clean C++ engine design

------------------------------------------------------------------------

## Future Improvements

Planned improvements include:

-   Animation system
-   Tilemap support
-   Entity Component System (ECS)
-   Improved physics and collision detection
-   Level editor
-   UI framework

------------------------------------------------------------------------

## Author

**Cameron Prybylski**

Software developer interested in:

-   Game engine development
-   Graphics programming
-   C++ systems programming

GitHub:\
https://github.com/CameronPrybylski
