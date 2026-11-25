cmake_minimum_required(VERSION 3.25)

add_library(engine-utils INTERFACE)

target_include_directories(engine-utils INTERFACE
    include
)

target_link_libraries(engine-utils INTERFACE
    project_warnings
)

target_compile_features(engine-utils INTERFACE cxx_std_20)
