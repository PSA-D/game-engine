cmake_minimum_required(VERSION 3.25)

find_package(Catch2 3 CONFIG REQUIRED)

add_executable(EngineTests
    test_engine.cpp
    test_game_object.cpp
    core/CompositionRootTest.cpp
    platform/HeadlessPlatformTest.cpp
    persistence/JsonSaveStrategyTest.cpp
    persistence/PersistenceTestData.h
    persistence/JsonSaveStrategyIntegrationTest.cpp
)

target_link_libraries(EngineTests PRIVATE
    engine
    project_sanitizers
    project_warnings
    Catch2::Catch2WithMain
)

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

target_include_directories(BackendTests PRIVATE ${PROJECT_SOURCE_DIR}/engine/src)

add_test(NAME BackendTests COMMAND BackendTests)

add_executable(RuntimeTests
    runtime/InputListenersTest.cpp
    runtime/DataNodeTest.cpp
    runtime/LevelTest.cpp
    runtime/RegionTest.cpp
    runtime/SceneTest.cpp
)

target_link_libraries(RuntimeTests PRIVATE
    engine
    project_sanitizers
    project_warnings
    Catch2::Catch2WithMain
)

target_include_directories(RuntimeTests PRIVATE ${PROJECT_SOURCE_DIR}/engine/src)

add_test(NAME RuntimeTests COMMAND RuntimeTests)

add_executable(InputTests
    input/InputHandlerTests.cpp
)

target_link_libraries(InputTests PRIVATE
    engine
    project_sanitizers
    project_warnings
    Catch2::Catch2WithMain
)

target_include_directories(InputTests PRIVATE ${PROJECT_SOURCE_DIR}/engine/src)

add_test(NAME InputTests COMMAND InputTests)

add_executable(IntegrationTests
    integration/InputSubsystemTests.cpp
)

target_link_libraries(IntegrationTests PRIVATE
    engine
    project_sanitizers
    project_warnings
    Catch2::Catch2WithMain
)

target_include_directories(IntegrationTests PRIVATE ${PROJECT_SOURCE_DIR}/engine/src)

add_test(NAME IntegrationTests COMMAND IntegrationTests)
