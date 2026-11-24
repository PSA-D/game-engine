cmake_minimum_required(VERSION 3.25)

add_library(engine STATIC
    src/core/Engine.cpp
        src/runtime/Level.cpp
        src/runtime/Level.h
        src/runtime/Region.cpp
        src/runtime/Region.h
        src/runtime/Scene.cpp
        src/runtime/Scene.h
)

target_include_directories(engine PUBLIC
    include
)

target_link_libraries(engine PUBLIC
    project_sanitizers
    project_warnings
)

target_compile_features(engine PUBLIC cxx_std_20)
