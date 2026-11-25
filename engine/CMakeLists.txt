cmake_minimum_required(VERSION 3.25)

add_library(engine STATIC
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
        src/runtime/Level.cpp
        src/runtime/Level.h
        src/runtime/Region.cpp
        src/runtime/Region.h
        src/runtime/Scene.cpp
        src/runtime/Scene.h
    src/utils/Interpolator.cpp
    src/utils/Rect.cpp
)

target_include_directories(engine
    PUBLIC include
    PRIVATE src
)

target_link_libraries(engine PUBLIC
    project_sanitizers
    project_warnings
)

target_compile_features(engine PUBLIC cxx_std_20)
