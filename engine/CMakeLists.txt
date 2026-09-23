cmake_minimum_required(VERSION 4.3)

add_library(engine STATIC
    src/core/Engine.cpp
)

target_include_directories(engine PUBLIC
    include
)

target_link_libraries(engine PUBLIC
    project_sanitizers
    project_warnings
)

target_compile_features(engine PUBLIC cxx_std_20)
