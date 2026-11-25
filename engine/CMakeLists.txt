cmake_minimum_required(VERSION 3.25)

find_package(SDL3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(engine STATIC
        src/core/Engine.cpp
        src/persistence/JsonSaveStrategy.cpp
        src/platform/Events.h
        src/platform/IInputSource.h
        src/platform/INativeEventListener.h
        src/backends/sdl3/SDL3InputSource.cpp
        src/backends/sdl3/SDL3InputSource.h
        src/backends/sdl3/SDL3Window.h
        src/backends/sdl3/SDL3KeyMap.cpp
        src/backends/sdl3/SDL3KeyMap.h
        src/runtime/IMouseListener.h
        src/runtime/IKeyListener.h
        src/input/ListenersList.h
        src/input/InputSubsystem.cpp
        src/input/InputHandler.cpp
        src/input/InputHandler.h
        src/input/InputTrigger.h
        src/input/Bindings.h
        src/input/InputState.h
        src/input/commands/ICommand.h
        src/input/commands/FunctionCommand.h
        src/platform/IWindow.h
        src/platform/WindowConfig.h
        src/utils/Interpolator.cpp
        src/utils/Rect.cpp
        src/runtime/Level.cpp
        src/runtime/Region.cpp
        src/runtime/Scene.cpp
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
