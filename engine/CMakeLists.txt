cmake_minimum_required(VERSION 3.25)

find_package(SDL3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(engine STATIC
    src/core/Engine.cpp
    src/runtime/Level.cpp
    src/runtime/Region.cpp
    src/runtime/Scene.cpp
    src/persistence/JsonSaveStrategy.cpp
    src/backends/sdl3/SDL3InputSource.cpp
    src/backends/sdl3/SDL3KeyMap.cpp
    src/utils/Interpolator.cpp
    src/utils/Rect.cpp
)


target_link_libraries(engine
        PUBLIC
        project_sanitizers
        project_warnings
        SDL3::SDL3

        PRIVATE
        nlohmann_json::nlohmann_json
)


target_include_directories(engine
        PUBLIC include
        PRIVATE src
)


target_compile_features(engine PUBLIC cxx_std_20)