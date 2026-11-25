cmake_minimum_required(VERSION 3.25)

find_package(Catch2 3 CONFIG REQUIRED)

add_executable(EngineTests
    test_engine.cpp
    core/CompositionRootTest.cpp
    platform/HeadlessPlatformTest.cpp
)

target_link_libraries(EngineTests PRIVATE
    engine
    project_sanitizers
    project_warnings
    Catch2::Catch2WithMain
)

# Tests mogen de interne headers van de engine gebruiken (CompositionRoot, platform, ...).
target_include_directories(EngineTests PRIVATE ${PROJECT_SOURCE_DIR}/engine/src)

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
