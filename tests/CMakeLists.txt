cmake_minimum_required(VERSION 3.25)

find_package(Catch2 CONFIG REQUIRED)

add_executable(EngineTests
        test_engine.cpp
        backends/sdl3/SDL3KeyMapTest.cpp
        backends/sdl3/SDL3InputSourceTest.cpp
        runtime/InputListenersTest.cpp
        input/InputHandlerTests.cpp
        integration/input/InputSubsystemTests.cpp
        integration/input/SDL3InputTests.cpp
)

target_link_libraries(EngineTests PRIVATE engine)

target_link_libraries(EngineTests PRIVATE
        project_sanitizers
        project_warnings
        Catch2::Catch2
        Catch2::Catch2WithMain
        engine
)

target_link_directories(EngineTests PRIVATE ../engine/cmake-build-debug)
target_include_directories(EngineTests PRIVATE ../engine)

add_test(NAME EngineTests COMMAND EngineTests)
