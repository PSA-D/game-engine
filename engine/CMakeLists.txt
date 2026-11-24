cmake_minimum_required(VERSION 3.25)

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
    src/core/Region.cpp
    src/core/Transform.cpp
)

target_include_directories(engine PUBLIC
    include
)

target_link_libraries(engine PUBLIC
    project_sanitizers
    project_warnings
)

target_compile_features(engine PUBLIC cxx_std_20)
