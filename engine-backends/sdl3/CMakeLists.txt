cmake_minimum_required(VERSION 3.25)

find_package(SDL3 CONFIG REQUIRED)

add_library(engine-sdl3-renderer STATIC
    src/SDL3Renderer.cpp
    src/SDL3Window.cpp
)

target_include_directories(engine-sdl3-renderer PUBLIC
    include
)

target_link_libraries(engine-sdl3-renderer PUBLIC
    engine-platform
    project_sanitizers
    project_warnings
    SDL3::SDL3
)

target_compile_features(engine-sdl3-renderer PUBLIC cxx_std_20)
