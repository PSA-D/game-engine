cmake_minimum_required(VERSION 3.25)

add_executable(EngineTests
    test_engine.cpp
        persistence/JsonSaveStrategyTest.cpp
        runtime/DataNodeTest.cpp
        persistence/PersistenceTestData.h
)

target_link_libraries(EngineTests PRIVATE engine)

target_link_libraries(EngineTests PRIVATE
    project_sanitizers
    project_warnings
        Catch2::Catch2WithMain
        engine
)

target_link_directories(EngineTests PRIVATE ../engine/cmake-build-debug)
target_include_directories(EngineTests PRIVATE ../engine)

add_test(NAME EngineTests COMMAND EngineTests)
