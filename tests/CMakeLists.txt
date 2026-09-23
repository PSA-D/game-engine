cmake_minimum_required(VERSION 4.3)

add_executable(EngineTests
    test_engine.cpp
)

target_link_libraries(EngineTests PRIVATE engine)

target_link_libraries(EngineTests PRIVATE
    project_sanitizers
    project_warnings
)

target_link_directories(EngineTests PRIVATE ../engine/cmake-build-debug)
target_include_directories(EngineTests PRIVATE ../engine)

add_test(NAME EngineTests COMMAND EngineTests)
