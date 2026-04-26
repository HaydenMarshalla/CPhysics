# CPhysics

CPhysics is a small 2D rigid body physics engine written in C++17 for game-style simulations. It includes collision detection, contact solving, spring joints, ray casting, explosions, object slicing, and an OpenGL/ImGui testbed for interactive demos.

## Features

- Circle and convex polygon rigid bodies
- Static and dynamic bodies with mass, inertia, friction, restitution, forces, torque, and gravity
- Circle-circle, circle-polygon, and polygon-polygon collision detection
- Sequential impulse contact solver with friction, restitution, positional correction, and Baumgarte-style velocity bias
- Spring-style joints between bodies and from bodies to fixed points
- Ray casting, ray scatter queries, shadow/line-of-sight helpers, slicing, and explosion demos
- Simple spatial-hash collision broadphase
- Interactive OpenGL testbed with 15 demos and runtime solver/debug controls
- Framework-free geometry regression tests runnable through CTest

## Build And Test

From the repository root:

```powershell
cmake -S . -B build
cmake --build build --config Release --parallel
ctest --test-dir build --build-config Release --output-on-failure
```

With warnings treated as errors:

```powershell
cmake -S . -B build-werror -DCPHYSICS_WARNINGS_AS_ERRORS=ON
cmake --build build-werror --config Release --parallel
ctest --test-dir build-werror --build-config Release --output-on-failure
```

Build outputs are written to `build/bin/`.

## Run The Testbed

The testbed expects its working directory to be `Rebuild of cphysics/Rebuild of cphysics/testbed` because settings are still loaded from a relative path.

From PowerShell:

```powershell
cd "Rebuild of cphysics\Rebuild of cphysics\testbed"
..\..\..\build\bin\cphysics_testbed.exe
```

To run only the regression executable directly:

```powershell
.\build\bin\cphysics_geometry_regression.exe
```

## Project Layout

```text
Rebuild of cphysics/Rebuild of cphysics/
  include/CPhysics/        Core engine headers
  src/                     Core engine implementation
  testbed/                 OpenGL visualization, ImGui UI, and demos
  testbed/Tests/           Interactive physics scenarios
  dependencies/            Vendored GLAD, ImGui, and cereal headers/sources
  tests/                   Framework-free regression tests
```

Top-level CMake adds the nested engine directory and produces:

- `cphysics` static library
- `cphysics_testbed` interactive demo application
- `cphysics_geometry_regression` regression test executable

## Recent Stability Work

The engine has been hardened in several important areas:

- Owning raw pointers were migrated to `std::unique_ptr<Body>`, `std::unique_ptr<Shape>`, and `std::unique_ptr<Joint>`.
- `Body` and `World` copying is disabled to avoid double-free and use-after-free bugs.
- Removing a body now removes joints that reference it.
- Runtime validation was added for shapes, bodies, rays, slices, explosions, world stepping, and settings IO.
- Degenerate polygons, invalid radii, invalid densities, bad ray distances, and zero-count explosions now fail with exceptions instead of silently producing NaNs.
- Collision now uses a simple spatial-hash broadphase before narrowphase checks.
- Regression tests cover geometry edge cases, invalid inputs, broadphase behavior, joint removal, slicing, and contact stabilization.
- Testbed rendering now batches per frame instead of flushing after every body.

## Important Caveats

- Raycast and shadow/line-of-sight queries still scan the full body list. The collision broadphase does not accelerate ray queries yet.
- The testbed uses large world-unit objects with gravity set to `-9.81`, so some demos can look floaty unless gravity/impulses are retuned or a real fixed-timestep accumulator is added.
- Joints still apply spring tension as impulses without timestep scaling, so joint behavior remains frame-rate dependent.
- Many engine fields are public and mutable, so callers can still bypass invariants after construction.
- The settings file path is still relative to the testbed working directory.
- The repository currently preserves the original nested `Rebuild of cphysics/Rebuild of cphysics` layout.

## Dependencies

- CMake 3.20+
- C++17 compiler: MSVC, GCC, or Clang
- OpenGL
- GLFW 3.4, resolved by `find_package(glfw3)` when available or fetched by CMake FetchContent otherwise
- Vendored GLAD, Dear ImGui, and cereal

## Screenshots

![Shadow casting](Images/Shadow%20casting%20c++.PNG)
![Chains](Images/Chains%20c++.PNG)
![Slicing](Images/slice%20c++.PNG)
