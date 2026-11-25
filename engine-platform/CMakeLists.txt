cmake_minimum_required(VERSION 3.25)

add_library(engine-platform INTERFACE)

target_include_directories(engine-platform INTERFACE
    include
)

target_link_libraries(engine-platform INTERFACE
    engine-utils
    project_warnings
)

target_compile_features(engine-platform INTERFACE cxx_std_20)
