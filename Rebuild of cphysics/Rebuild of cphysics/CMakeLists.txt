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

find_package(OpenGL REQUIRED)

set(CPHYSICS_USE_VENDORED_GLFW_DEFAULT OFF)
if(MSVC AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CPHYSICS_USE_VENDORED_GLFW_DEFAULT ON)
endif()

option(
    CPHYSICS_USE_VENDORED_GLFW
    "Use the vendored MSVC x64 GLFW binary for the testbed"
    ${CPHYSICS_USE_VENDORED_GLFW_DEFAULT}
)

add_executable(cphysics_testbed
    testbed/Main.cpp
    testbed/Render.cpp
    testbed/Test.cpp
    testbed/imgui/imgui.cpp
    testbed/imgui/imgui_draw.cpp
    testbed/imgui/imgui_widgets.cpp
    testbed/imgui/imgui_impl_glfw.cpp
    testbed/imgui/imgui_impl_opengl3.cpp
    dependencies/GLAD/src/glad.c
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
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies"
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLAD/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/imgui"
        "${CMAKE_CURRENT_SOURCE_DIR}/testbed"
)
target_link_libraries(cphysics_testbed PRIVATE cphysics OpenGL::GL)
set_target_properties(cphysics_testbed
    PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/testbed"
)

if(CPHYSICS_USE_VENDORED_GLFW)
    if(NOT MSVC)
        message(FATAL_ERROR "CPHYSICS_USE_VENDORED_GLFW requires MSVC because dependencies/GLFW/lib-vc2019 contains MSVC binaries.")
    endif()
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
        message(FATAL_ERROR "CPHYSICS_USE_VENDORED_GLFW requires a 64-bit build because the vendored GLFW binary is MSVC x64 only.")
    endif()

    set(CPHYSICS_VENDORED_GLFW_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/GLFW")
    add_library(cphysics_vendored_glfw STATIC IMPORTED)
    set_target_properties(cphysics_vendored_glfw
        PROPERTIES
            IMPORTED_LOCATION "${CPHYSICS_VENDORED_GLFW_DIR}/lib-vc2019/glfw3.lib"
            INTERFACE_INCLUDE_DIRECTORIES "${CPHYSICS_VENDORED_GLFW_DIR}/include"
    )
    target_link_libraries(cphysics_testbed PRIVATE cphysics_vendored_glfw)
    add_custom_command(
        TARGET cphysics_testbed
        POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "${CPHYSICS_VENDORED_GLFW_DIR}/lib-vc2019/glfw3.dll"
            "$<TARGET_FILE_DIR:cphysics_testbed>/glfw3.dll"
        VERBATIM
    )
else()
    find_package(glfw3 CONFIG QUIET)
    if(TARGET glfw)
        target_link_libraries(cphysics_testbed PRIVATE glfw)
    elseif(TARGET glfw3)
        target_link_libraries(cphysics_testbed PRIVATE glfw3)
    else()
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
        target_link_libraries(cphysics_testbed PRIVATE glfw)
    endif()
endif()

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

# UnitTests uses Microsoft.VisualStudio.CppUnitTestFramework and remains covered by
# the checked-in Visual Studio solution; it is intentionally not built by CMake.
