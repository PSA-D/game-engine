cmake_minimum_required(VERSION 3.25)

add_library(engine-null-renderer STATIC
    src/NullRenderer.cpp
)

target_include_directories(engine-null-renderer PUBLIC
    include
)

target_link_libraries(engine-null-renderer PUBLIC
    engine-platform
    project_sanitizers
    project_warnings
)

target_compile_features(engine-null-renderer PUBLIC cxx_std_20)
