cmake_minimum_required(VERSION 3.25)

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

add_executable(UtilsTests
    test_utils.cpp
)

target_link_libraries(UtilsTests PRIVATE
    engine
    project_sanitizers
    project_warnings
)

add_test(NAME UtilsTests COMMAND UtilsTests)
