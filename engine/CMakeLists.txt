cmake_minimum_required(VERSION 3.25)

add_library(engine STATIC
    src/core/Engine.cpp
        src/persistence
)

find_package(nlohmann_json CONFIG REQUIRED)

target_include_directories(engine PUBLIC
    include
)

target_link_libraries(engine PUBLIC
    project_sanitizers
    project_warnings
        PRIVATE
        nlohmann_json::nlohmann_json
)

target_compile_features(engine PUBLIC cxx_std_20)
