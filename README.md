 This is a 2D rigid body physics engine written in C++ for game development, featuring collision detection, constraint solving, and advanced capabilities like
  explosions and object slicing.

  Tech Stack

  - Core: C++ with minimal dependencies
  - Rendering: OpenGL 3.3+, GLAD, GLFW
  - UI: Dear ImGui
  - Serialization: Cereal, RapidJSON
  - Build: CMake 3.20+ (any compiler with C++17 support — MSVC, GCC, Clang)
  - Testing: CTest

## Build

From the repository root:

```sh
cmake -S . -B build
cmake --build build --config Release --parallel
ctest --test-dir build --build-config Release --output-on-failure
```

Binaries are written to `build/bin/`. The `cphysics_testbed` executable expects
its working directory to be `Rebuild of cphysics/Rebuild of cphysics/testbed/`
so that `settings.bin` and `imgui.ini` resolve via the relative paths used in
`Main.cpp`. When opening the generated VS solution, this is set automatically.

GLFW is resolved via `find_package(glfw3)` if available on the system,
otherwise CMake builds it from source via FetchContent (GLFW 3.4).

Optional CMake flags:
- `-DCPHYSICS_WARNINGS_AS_ERRORS=ON` — treat warnings as errors (used in CI).


  Project Structure

  Rebuild of cphysics/
  ├── include/CPhysics/          # Core physics headers (20 files)
  ├── src/                       # Physics implementation (~1,562 LOC)
  ├── testbed/                   # OpenGL visualization + demos
  │   ├── Tests/                 # 15 physics demonstration scenarios
  │   └── imgui/                 # Dear ImGui integration
  ├── dependencies/              # Third-party libraries (GLAD, ImGui, cereal)
  └── tests/                     # Framework-free regression tests (run via ctest)

  Core Components

  Physics Engine:
  - World.h/cpp - Main simulation container, time-stepping, gravity
  - Body.h/cpp - Rigid bodies with mass, velocity, forces, torque
  - Shape.h - Abstract base (Circle, Polygon implementations)
  - Arbiter.h/cpp - Collision detection and contact resolution
  - Joint.h - Constraint system (body-to-body, body-to-point springs)

  Collision System:
  - Broadphase: AABB overlap testing
  - Narrowphase: Circle-circle, circle-polygon, polygon-polygon
  - Sequential Impulses solver with friction and restitution
  - Contact manifolds (up to 2 contact points)

  Advanced Features:
  - Slice.h - Cut objects along arbitrary lines
  - ParticleExplosion.h, ProximityExplosion.h, RaycastExplosion.h - 3 explosion types
  - Shadowcast.h - Visibility/shadow calculations
  - Ray.h - Ray casting for queries

  Testbed Demos (15 scenarios):
  - Chains, Trebuchet, Newton's Cradle
  - Friction, Restitution, Drag testing
  - Stack, Bouncing ball, Wrecking ball
  - All 3 explosion types, Object slicing
  - Line of sight, Ray casting

  Architecture Highlights

  1. Polymorphic shape system - Virtual base class for extensibility
  2. Sequential Impulses solver - Iterative constraint resolution
  3. Spring-based joints - Configurable stiffness and damping
  4. AABB optimization - Broadphase filtering before collision checks
  5. Data-driven config - Serializable settings with ImGui controls

  Key Features

  - Pure C++ implementation (no external physics library)
  - Convex polygon support (arbitrary vertex count)
  - Stable object stacking
  - Momentum conservation
  - Static/dynamic friction
  - Customizable restitution (bounciness)
  - Real-time parameter tuning via GUI
  - Save/load settings

  The testbed (testbed/Main.cpp) provides an interactive OpenGL window where you can run demos, visualize physics (AABBs, contacts, centers of mass), and tweak solver
  parameters in real-time.

  Build (CMake)

  From the repository root:

  ```powershell
  cmake -S . -B build
  cmake --build build --config Release
  ctest --test-dir build -C Release
  ```

  CMake produces `cphysics`, `cphysics_testbed`, and `cphysics_geometry_regression`. The testbed keeps the Visual Studio working-directory behavior; when running it manually outside Visual Studio, use the `testbed/` directory as the working directory so `../Rebuild of cphysics/testbed/settings.bin` resolves as expected.
  
## Screenshots
![a relative link](Images/Shadow%20casting%20c++.PNG)
![a relative link](Images/Chains%20c++.PNG)
![a relative link](Images/slice%20c++.PNG)
