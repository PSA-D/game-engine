cmake_minimum_required(VERSION 3.25)

find_package(SDL3 CONFIG REQUIRED)

add_library(engine STATIC
        src/core/Engine.cpp
        src/utils/Vector2.h
        src/utils/InputCodes.h
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
        src/input/commands/ICommand.h
        src/input/InputTrigger.h
        src/input/Bindings.h
        src/input/InputState.h
)

target_include_directories(engine PUBLIC
        include
        src
)

target_link_libraries(engine PUBLIC
        project_sanitizers
        project_warnings
        SDL3::SDL3
)

target_compile_features(engine PUBLIC cxx_std_20)
