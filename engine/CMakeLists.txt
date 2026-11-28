cmake_minimum_required(VERSION 3.25)

find_package(SDL3 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_library(engine STATIC
        # Publieke headers (voor de game)
        include/core/Engine.h
        include/core/EngineConfig.h
        include/facades/Time.h
        include/runtime/Component.h
        include/runtime/DataNode.h
        include/runtime/GameObject.h
        include/runtime/IKeyListener.h
        include/runtime/IMouseListener.h
        include/runtime/Level.h
        include/runtime/Region.h
        include/runtime/Scene.h
        include/runtime/Transform.h
        include/runtime/SceneManager.h
        include/runtime/EventBus.h
        include/runtime/Subscription.h
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

        # Core
        src/core/CompositionRoot.cpp
        src/core/CompositionRoot.h
        src/core/Engine.cpp
        src/core/EngineImpl.cpp
        src/core/EngineImpl.h
        src/core/GameLoop.cpp
        src/core/GameLoop.h

        # Runtime
        src/runtime/Component.cpp
        src/runtime/GameObject.cpp
        src/runtime/Level.cpp
        src/runtime/Region.cpp
        src/runtime/Scene.cpp
        src/runtime/Transform.cpp
        src/runtime/EventBus.cpp
        src/runtime/Subscription.cpp
        src/runtime/SceneManager.cpp

        # Platform
        src/platform/Events.h
        src/platform/IInputSource.h
        src/platform/INativeEventListener.h

        # Backends
        src/backends/sdl3/SDL3InputSource.cpp
        src/backends/sdl3/SDL3InputSource.h
        src/backends/sdl3/SDL3KeyMap.cpp
        src/backends/sdl3/SDL3KeyMap.h
        src/backends/sdl3/SDL3Window.h
        src/backends/sdl3/SDL3PlatformFactory.cpp
        src/backends/sdl3/SDL3PlatformFactory.h
        src/backends/sdl3/SDL3Window.cpp

        # Subsystems: input
        src/subsystems/input/Bindings.h
        src/subsystems/input/InputHandler.cpp
        src/subsystems/input/InputHandler.h
        src/subsystems/input/InputState.h
        src/subsystems/input/InputSubsystem.cpp
        src/subsystems/input/InputSubsystem.h
        src/subsystems/input/InputTrigger.h
        src/subsystems/input/ListenersList.h
        src/subsystems/input/commands/FunctionCommand.h
        src/subsystems/input/commands/ICommand.h

        # Subsystems: persistence
        src/subsystems/persistence/GameData.h
        src/subsystems/persistence/ISaveStrategy.h
        src/subsystems/persistence/JsonSaveStrategy.cpp
        src/subsystems/persistence/JsonSaveStrategy.h

        # Overige subsystems
        src/subsystems/ai/AISubsystem.h
        src/subsystems/animation/AnimationSubsystem.h
        src/subsystems/audio/AudioSubsystem.h
        src/subsystems/debug/DebugSubsystem.h
        src/subsystems/editor/LevelEditor.h
        src/subsystems/input/InputSubsystem.h
        src/subsystems/physics/PhysicsSubsystem.h
        src/subsystems/render/RenderSubsystem.h
        src/subsystems/ui/UISubsystem.h

        # Platform
        src/platform/IPlatformFactory.h
        src/platform/IWindow.h
        src/platform/WindowConfig.h
        src/platform/headless/HeadlessInputSource.cpp
        src/platform/headless/HeadlessInputSource.h
        src/platform/headless/HeadlessPlatformFactory.cpp
        src/platform/headless/HeadlessPlatformFactory.h
        src/platform/headless/HeadlessWindow.cpp
        src/platform/headless/HeadlessWindow.h

        # Utils
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
        # Backends en 3rd party zijn een implementatiedetail: de game ziet ze niet (package-diagram).
        PRIVATE
        SDL3::SDL3
        nlohmann_json::nlohmann_json
)

target_compile_features(engine PUBLIC cxx_std_20)
