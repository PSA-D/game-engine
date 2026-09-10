add_executable(EngineTests
    test_engine.cpp
)

target_link_libraries(EngineTests PRIVATE engine)

add_test(NAME EngineTests COMMAND EngineTests)
