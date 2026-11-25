cmake_minimum_required(VERSION 3.25)

find_package(SDL3 CONFIG REQUIRED)

add_library(engine STATIC
    include/Engine.h
    include/EngineConfig.h
    include/Scene.h
    include/SceneManager.h
    include/facades/Time.h
    include/utils/AssetHandle.h
    include/utils/BodyType.h
    include/utils/Color.h
    include/utils/Easing.h
    include/utils/Interpolator.h
    include/utils/KeyCode.h
    include/utils/Matrix.h
    include/utils/MouseButton.h
    include/utils/Rect.h
    include/utils/Size.h
    include/utils/Vector2.h
    src/core/CompositionRoot.cpp
    src/core/CompositionRoot.h
    src/core/Engine.cpp
    src/core/EngineImpl.cpp
    src/core/EngineImpl.h
    src/core/GameLoop.cpp
    src/core/GameLoop.h
    src/subsystems/ai/AISubsystem.h
    src/subsystems/animation/AnimationSubsystem.h
    src/subsystems/audio/AudioSubsystem.h
    src/subsystems/debug/DebugSubsystem.h
    src/subsystems/editor/LevelEditor.h
    src/subsystems/input/InputSubsystem.h
    src/subsystems/physics/PhysicsSubsystem.h
    src/subsystems/render/RenderSubsystem.h
    src/subsystems/ui/UISubsystem.h
    src/platform/IPlatformFactory.h
    src/platform/IWindow.h
    src/platform/WindowConfig.h
    src/platform/headless/HeadlessPlatformFactory.cpp
    src/platform/headless/HeadlessPlatformFactory.h
    src/platform/headless/HeadlessWindow.cpp
    src/platform/headless/HeadlessWindow.h
    src/backends/sdl3/SDL3PlatformFactory.cpp
    src/backends/sdl3/SDL3PlatformFactory.h
    src/backends/sdl3/SDL3Window.cpp
    src/backends/sdl3/SDL3Window.h
    src/utils/Interpolator.cpp
    src/utils/Rect.cpp
)

target_include_directories(engine
    PUBLIC include
    PRIVATE src
)

target_link_libraries(engine
    PUBLIC
        project_sanitizers
        project_warnings
    # Backends zijn een implementatiedetail: de game ziet SDL niet (package-diagram).
    PRIVATE
        SDL3::SDL3
)

target_compile_features(engine PUBLIC cxx_std_20)
