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
g++ -std=c++17 -I"Rebuild of cphysics/Rebuild of cphysics/include" "Rebuild of cphysics/Rebuild of cphysics/tests/GeometryRegression.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/Arbiter.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/Body.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/Circle.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/Joint.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/JointToBody.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/JointToPoint.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/Polygon.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/Ray.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/Slice.cpp" "Rebuild of cphysics/Rebuild of cphysics/src/World.cpp" -o ".codex-build/GeometryRegression.exe"
.\.codex-build\GeometryRegression.exe
```
