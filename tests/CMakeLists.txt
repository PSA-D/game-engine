cmake_minimum_required(VERSION 3.25)

find_package(Catch2 REQUIRED)

add_executable(EngineTests
    test_game_object.cpp
    test_engine.cpp
    test_scene.cpp
)

target_link_libraries(EngineTests PRIVATE engine)

target_link_libraries(EngineTests PRIVATE
    Catch2::Catch2WithMain
    project_sanitizers
    project_warnings
)

target_include_directories(EngineTests PRIVATE ../engine)

add_test(NAME EngineTests COMMAND EngineTests)
