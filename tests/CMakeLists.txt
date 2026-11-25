cmake_minimum_required(VERSION 3.25)

find_package(Catch2 3 CONFIG REQUIRED)

add_executable(EngineTests
    test_engine.cpp

    persistence/JsonSaveStrategyTest.cpp
    runtime/DataNodeTest.cpp
    persistence/PersistenceTestData.h
    persistence/JsonSaveStrategyIntegrationTest.cpp
)

target_link_libraries(EngineTests PRIVATE
    engine
    project_sanitizers
    project_warnings
    Catch2::Catch2WithMain
)

target_link_directories(EngineTests PRIVATE ../engine/cmake-build-debug)

target_include_directories(EngineTests PRIVATE
    ../engine
    ../engine/src
)

add_test(NAME EngineTests COMMAND EngineTests)

add_executable(UtilsTests
    utils/AssetHandleTest.cpp
    utils/ColorTest.cpp
    utils/InterpolatorTest.cpp
    utils/MatrixTest.cpp
    utils/RectTest.cpp
    utils/Vector2Test.cpp
)

target_link_libraries(UtilsTests PRIVATE
    engine
    project_sanitizers
    project_warnings
    Catch2::Catch2WithMain
)

add_test(NAME UtilsTests COMMAND UtilsTests)

add_executable(BackendTests
    backends/sdl3/SDL3KeyMapTest.cpp
    backends/sdl3/SDL3InputSourceTest.cpp
)

target_link_libraries(BackendTests PRIVATE
    engine
    project_sanitizers
    project_warnings
    Catch2::Catch2WithMain
)

target_include_directories(BackendTests PRIVATE ../engine/src)

add_test(NAME BackendTests COMMAND BackendTests)