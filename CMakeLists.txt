cmake_minimum_required(VERSION 3.20.0)

project(ray_tracing
    VERSION 1.0
    DESCRIPTION "Ray trace project"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(ray_tracing
    src/main.cc)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # target_compile_options(ray_tracing PRIVATE /W4)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(ray_tracing PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # other macOS-specific flags for Clang
endif()

target_compile_features(ray_tracing PRIVATE cxx_std_17)