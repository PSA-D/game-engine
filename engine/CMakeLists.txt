cmake_minimum_required(VERSION 3.25)

find_package(SDL3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(engine STATIC
    include/Component.h
    include/Engine.h
    include/EngineConfig.h
    include/GameObject.h
    include/Region.h
    include/Transform.h
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
    include/utils/Vector2.h
    src/core/Component.cpp
    src/core/CompositionRoot.cpp
    src/core/CompositionRoot.h
    src/core/Engine.cpp
    src/core/EngineImpl.cpp
    src/core/EngineImpl.h
    src/core/GameObject.cpp
    src/core/Transform.cpp
    src/persistence/GameData.h
    src/persistence/ISaveStrategy.h
    src/persistence/JsonSaveStrategy.cpp
    src/persistence/JsonSaveStrategy.h
    src/platform/Events.h
    src/platform/IInputSource.h
    src/platform/INativeEventListener.h
    src/core/GameLoop.cpp
    src/core/GameLoop.h
    src/subsystems/ai/AISubsystem.h
    src/subsystems/animation/AnimationSubsystem.h
    src/subsystems/audio/AudioSubsystem.h
    src/subsystems/debug/DebugSubsystem.h
    src/subsystems/editor/LevelEditor.h
    src/subsystems/input/InputSubsystem.h
    src/subsystems/physics/PhysicsSubsystem.h
    src/subsystems/ui/UISubsystem.h
    src/platform/IPlatformFactory.h
    src/backends/sdl3/SDL3InputSource.cpp
    src/backends/sdl3/SDL3InputSource.h
    src/backends/sdl3/SDL3KeyMap.cpp
    src/backends/sdl3/SDL3KeyMap.h
    src/backends/sdl3/SDL3PlatformFactory.cpp
    src/backends/sdl3/SDL3PlatformFactory.h
    src/backends/sdl3/EngineSDL3Window.cpp
    src/backends/sdl3/EngineSDL3Window.h
    src/platform/headless/HeadlessPlatformFactory.cpp
    src/platform/headless/HeadlessPlatformFactory.h
    src/platform/headless/HeadlessWindow.cpp
    src/platform/headless/HeadlessWindow.h
    src/input/Bindings.h
    src/input/InputHandler.cpp
    src/input/InputHandler.h
    src/input/InputState.h
    src/input/InputSubsystem.cpp
    src/input/InputSubsystem.h
    src/input/InputTrigger.h
    src/input/ListenersList.h
    src/input/commands/FunctionCommand.h
    src/input/commands/ICommand.h
    src/runtime/DataNode.h
    src/runtime/IKeyListener.h
    src/runtime/IMouseListener.h
    src/runtime/Level.cpp
    src/runtime/Level.h
    src/runtime/Region.cpp
    src/runtime/Region.h
    src/runtime/Scene.cpp
    src/runtime/Scene.h
    src/utils/Interpolator.cpp
    src/utils/Rect.cpp
    include/core/Component.h
    include/core/GameObject.h
    include/components/SpriteRenderer.h
    include/subsystems/render/RenderSubsystem.h
    include/systems/render/Camera.h
    include/systems/render/CameraView.h
    include/systems/render/Culler.h
    include/systems/render/RenderItem.h
    include/systems/render/RenderQueue.h
    include/systems/RenderSystem.h
    src/components/SpriteRenderer.cpp
    src/subsystems/render/RenderSubsystem.cpp
    src/systems/render/Culler.cpp
    src/systems/render/RenderQueue.cpp
    src/systems/RenderSystem.cpp
)

target_include_directories(engine
    PUBLIC include
    PRIVATE src
)

target_link_libraries(engine
    PUBLIC
        project_sanitizers
        project_warnings
        engine-platform
    PRIVATE
        SDL3::SDL3
        nlohmann_json::nlohmann_json
)


target_compile_features(engine PUBLIC cxx_std_20)
