cmake_minimum_required(VERSION 3.20.0)

project (ray_tracing
    VERSION 1.0
    DESCRIPTION "Ray trace project"
    LANGUAGES CXX
)

# Set to C++17
set( CMAKE_CXX_STANDARD 17 )

# Source
set ( SOURCE_COMMON
    src/common/camera.h
    src/common/utility.h
    src/common/ray.h
    src/common/color.h
    src/common/vec3.h 
)

set ( SOURCE_RAY_TRACING
    ${SOURCE_COMMON}
    src/hittable_list.h
    src/sphere.h
    src/hittable.h
    src/main.cc
)

# Excutable
add_executable(ray_tracing  ${SOURCE_RAY_TRACING})

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # target_compile_options(ray_tracing PRIVATE /W4)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(ray_tracing PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # other macOS-specific flags for Clang
endif()

target_compile_features(ray_tracing PRIVATE cxx_std_17)

include_directories(src/common)