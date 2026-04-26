# Geometry Regression Tests

These tests are intentionally framework-free so the core engine can be checked without launching the OpenGL testbed.

From the repository root, build and run with CMake:

```powershell
cmake -S . -B build
cmake --build build --config Release --target cphysics_geometry_regression
ctest --test-dir build -C Release
```

From the repository root, build and run with MinGW g++:

```powershell
New-Item -ItemType Directory -Force -Path .codex-build
g++ -std=c++17 -I"include" tests/GeometryRegression.cpp src/Arbiter.cpp src/Body.cpp src/Circle.cpp src/Joint.cpp src/JointToBody.cpp src/JointToPoint.cpp src/ParticleExplosion.cpp src/Polygon.cpp src/ProximityExplosion.cpp src/Ray.cpp src/RaycastExplosion.cpp src/Rayscatter.cpp src/Shadowcast.cpp src/Slice.cpp src/World.cpp -o ".codex-build/GeometryRegression.exe"
.\.codex-build\GeometryRegression.exe
```
