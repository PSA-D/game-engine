cmake_minimum_required(VERSION 3.25)

add_library(engine STATIC
    src/core/Engine.cpp
        src/platform/VoiceHandle.h
        src/platform/AudioData.h
        src/platform/ITexture.h
        src/platform/IAudioClip.h
        src/platform/INativeEventListener.h
        src/platform/IPlatformPaths.h
        src/platform/IWindow.h
        src/platform/IRenderer.h
        src/platform/IInputSource.h
        src/platform/IAudioBackend.h
        src/platform/IPlatformFactory.h
)

target_include_directories(engine PUBLIC
    include
)

target_link_libraries(engine PUBLIC
    project_sanitizers
    project_warnings
)

target_compile_features(engine PUBLIC cxx_std_20)
