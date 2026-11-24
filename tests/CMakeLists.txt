cmake_minimum_required(VERSION 3.25)

find_package(Catch2 CONFIG REQUIRED)

add_executable(EngineTests
    test_engine.cpp
        runtime/LevelTest.cpp
        runtime/RegionTest.cpp
        runtime/SceneTest.cpp
        runtime/SceneLoadingIntegrationTest.cpp
)

target_link_libraries(EngineTests
        PRIVATE
        engine
        Catch2::Catch2WithMain
)

target_link_libraries(EngineTests PRIVATE
    project_sanitizers
    project_warnings
)

target_link_directories(EngineTests PRIVATE ../engine/cmake-build-debug)
target_include_directories(EngineTests PRIVATE ../engine)

add_test(NAME EngineTests COMMAND EngineTests)
