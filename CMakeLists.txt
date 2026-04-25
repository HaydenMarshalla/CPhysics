cmake_minimum_required(VERSION 3.20)

project(CPhysics LANGUAGES CXX C)

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

enable_testing()

add_subdirectory("Rebuild of cphysics/Rebuild of cphysics")
