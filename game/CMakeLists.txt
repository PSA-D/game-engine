cmake_minimum_required(VERSION 4.3)

add_executable(Game
    src/main.cpp
)

target_link_libraries(Game PRIVATE engine)

target_link_libraries(Game PRIVATE
    project_sanitizers
    project_warnings
)

target_include_directories(Game PRIVATE ../engine)
target_link_directories(Game PRIVATE ../engine/cmake-build-debug)
