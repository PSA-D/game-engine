cmake_minimum_required(VERSION 3.25)

add_library(engine STATIC
    include/Engine.h
    src/core/Engine.cpp
    src/utils/AssetHandle.h
    src/utils/BodyType.h
    src/utils/Color.h
    src/utils/Easing.h
    src/utils/Interpolator.cpp
    src/utils/Interpolator.h
    src/utils/KeyCode.h
    src/utils/Matrix.h
    src/utils/MouseButton.h
    src/utils/Rect.cpp
    src/utils/Rect.h
    src/utils/Vector2.h
)

target_include_directories(engine PUBLIC
    include
    src
)

target_link_libraries(engine PUBLIC
    project_sanitizers
    project_warnings
)

target_compile_features(engine PUBLIC cxx_std_20)
