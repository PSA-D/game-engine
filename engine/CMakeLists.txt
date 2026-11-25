cmake_minimum_required(VERSION 3.25)

find_package(SDL3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(engine STATIC
        src/core/Engine.cpp
        src/persistence/JsonSaveStrategy.cpp
)

target_link_libraries(engine PUBLIC
    project_sanitizers
    project_warnings
    SDL3::SDL3
        PRIVATE
        nlohmann_json::nlohmann_json
        src/platform/Events.h
        src/platform/IInputSource.h
        src/platform/INativeEventListener.h
        src/backends/sdl3/SDL3InputSource.cpp
        src/backends/sdl3/SDL3InputSource.h
        src/backends/sdl3/SDL3Window.h
        src/backends/sdl3/SDL3KeyMap.cpp
        src/backends/sdl3/SDL3KeyMap.h
        include/Engine.h
        include/utils/AssetHandle.h
        include/utils/BodyType.h
        include/utils/Color.h
        include/utils/Easing.h
        include/utils/Interpolator.h
        include/utils/KeyCode.h
        include/utils/Matrix.h
        include/utils/MouseButton.h
        include/utils/Rect.h
        include/utils/Vector2.h
        src/core/Engine.cpp
        src/utils/Interpolator.cpp
        src/utils/Rect.cpp
)

target_include_directories(engine
        PUBLIC include
        PRIVATE src
)


target_compile_features(engine PUBLIC cxx_std_20)