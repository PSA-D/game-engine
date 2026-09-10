add_library(engine STATIC
    src/core/Engine.cpp
)

target_include_directories(engine PUBLIC
    include
)

target_compile_features(engine PUBLIC cxx_std_20)
