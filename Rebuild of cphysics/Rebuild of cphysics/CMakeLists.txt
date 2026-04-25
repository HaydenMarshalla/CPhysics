# ---- Engine ---------------------------------------------------------------

set(CPHYSICS_ENGINE_SOURCES
    src/Arbiter.cpp
    src/Body.cpp
    src/Circle.cpp
    src/Joint.cpp
    src/JointToBody.cpp
    src/JointToPoint.cpp
    src/ParticleExplosion.cpp
    src/Polygon.cpp
    src/ProximityExplosion.cpp
    src/Ray.cpp
    src/RaycastExplosion.cpp
    src/Rayscatter.cpp
    src/Shadowcast.cpp
    src/Slice.cpp
    src/World.cpp
)

add_library(cphysics STATIC ${CPHYSICS_ENGINE_SOURCES})
target_include_directories(cphysics
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_compile_features(cphysics PUBLIC cxx_std_17)
target_compile_options(cphysics
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)
if(CPHYSICS_WARNINGS_AS_ERRORS)
    target_compile_options(cphysics
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/WX>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
    )
endif()

# ---- Window/GL system deps (resolved once, used by imgui + testbed) -------

find_package(OpenGL REQUIRED)

find_package(glfw3 CONFIG QUIET)
if(NOT TARGET glfw AND NOT TARGET glfw3)
    include(FetchContent)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
    )
    FetchContent_MakeAvailable(glfw)
endif()

# Normalize: some packagers expose the target as `glfw3`, FetchContent uses `glfw`.
if(TARGET glfw)
    set(CPHYSICS_GLFW_TARGET glfw)
else()
    set(CPHYSICS_GLFW_TARGET glfw3)
endif()

# ---- Vendored third-party (no -Werror, no -Wall) --------------------------
#
# These targets compile vendored code that we do not maintain. They are
# intentionally excluded from the project's warning discipline so warnings
# in upstream sources cannot break the build.

add_library(cphysics_glad STATIC dependencies/GLAD/src/glad.c)
target_include_directories(cphysics_glad
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLAD/include"
)

add_library(cphysics_imgui STATIC
    testbed/imgui/imgui.cpp
    testbed/imgui/imgui_draw.cpp
    testbed/imgui/imgui_widgets.cpp
    testbed/imgui/imgui_impl_glfw.cpp
    testbed/imgui/imgui_impl_opengl3.cpp
)
target_include_directories(cphysics_imgui
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui"
)
# imgui_impl_opengl3.h auto-detects the GL loader from headers on the include
# path; expose GLAD publicly so consumers including that header pick it up.
target_link_libraries(cphysics_imgui PUBLIC ${CPHYSICS_GLFW_TARGET} cphysics_glad)

# ---- Testbed --------------------------------------------------------------

add_executable(cphysics_testbed
    testbed/Main.cpp
    testbed/Render.cpp
    testbed/Test.cpp
)
target_compile_features(cphysics_testbed PRIVATE cxx_std_17)
target_compile_options(cphysics_testbed
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)
if(CPHYSICS_WARNINGS_AS_ERRORS)
    target_compile_options(cphysics_testbed
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/WX>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
    )
endif()
target_include_directories(cphysics_testbed
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies"   # cereal
        "${CMAKE_CURRENT_SOURCE_DIR}/testbed"        # Settings.h, Colour.h, Tests/...
)
target_link_libraries(cphysics_testbed
    PRIVATE
        cphysics
        cphysics_glad
        cphysics_imgui
        ${CPHYSICS_GLFW_TARGET}
        OpenGL::GL
)
set_target_properties(cphysics_testbed
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/testbed"
)

# ---- Tests ----------------------------------------------------------------

add_executable(cphysics_geometry_regression tests/GeometryRegression.cpp)
target_compile_features(cphysics_geometry_regression PRIVATE cxx_std_17)
target_compile_options(cphysics_geometry_regression
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive->
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra>
)
if(CPHYSICS_WARNINGS_AS_ERRORS)
    target_compile_options(cphysics_geometry_regression
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/WX>
            $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Werror>
    )
endif()
target_link_libraries(cphysics_geometry_regression PRIVATE cphysics)
add_test(NAME geometry_regression COMMAND cphysics_geometry_regression)
