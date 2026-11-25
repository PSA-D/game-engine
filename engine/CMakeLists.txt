cmake_minimum_required(VERSION 3.25)

add_library(engine STATIC
    include/Component.h
    include/GameObject.h
    include/Region.h
    include/Transform.h
    include/EventBus.h
    include/Scene.h
    include/SceneManager.h
    include/utils/Matrix.h
    include/utils/Vector2.h
    src/Engine.cpp
    src/runtime/Component.cpp
    src/runtime/EventBus.cpp
    src/runtime/GameObject.cpp
    src/runtime/Region.cpp
    src/runtime/Scene.cpp
    src/runtime/SceneManager.cpp
    src/runtime/Transform.cpp
)

target_include_directories(engine PUBLIC
    include
)

target_link_libraries(engine PUBLIC
    project_sanitizers
    project_warnings
)

target_compile_features(engine PUBLIC cxx_std_20)
