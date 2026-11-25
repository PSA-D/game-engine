cmake_minimum_required(VERSION 3.25)

find_package(SDL3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(engine STATIC
    include/Component.h
    include/GameObject.h
    include/Region.h
    include/Transform.h
    include/utils/Matrix.h
    include/utils/Vector2.h
    src/core/Component.cpp
    src/core/Engine.cpp
    src/core/GameObject.cpp
    src/core/Transform.cpp
    src/runtime/Level.cpp
        src/runtime/Scene.cpp
    src/persistence/JsonSaveStrategy.cpp
    src/persistence/LevelSerializer.cpp
    src/platform/Events.h
    src/platform/IInputSource.h
    src/platform/INativeEventListener.h
    src/backends/sdl3/SDL3InputSource.cpp
    src/backends/sdl3/SDL3InputSource.h
    src/backends/sdl3/SDL3KeyMap.cpp
    src/backends/sdl3/SDL3KeyMap.h
    src/backends/sdl3/SDL3Window.h
    src/input/ListenersList.h
    src/input/InputSubsystem.cpp
    src/input/InputHandler.cpp
    src/input/InputHandler.h
    src/input/commands/ICommand.h
    src/input/InputTrigger.h
    src/input/Bindings.h
    src/input/InputState.h
    src/input/commands/FunctionCommand.h
    src/runtime/IMouseListener.h
    src/runtime/IKeyListener.h
    src/runtime/Level.cpp
    src/runtime/Scene.cpp
    src/utils/Interpolator.cpp
    src/utils/Rect.cpp
        src/core/ComponentRegistry.cpp
        src/core/ComponentRegistry.h
        src/core/Region.cpp
        src/core/Persistence.cpp
        src/core/Persistence.h
        src/persistence/PersistenceError.h
        src/persistence/PersistenceSubsystem.cpp
        src/persistence/PersistenceSubsystem.h
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
