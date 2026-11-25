cmake_minimum_required(VERSION 3.25)

add_executable(Game
        src/main.cpp
)

target_link_libraries(Game PRIVATE engine)
target_link_libraries(Game PRIVATE
    engine-null-renderer
    engine-sdl3-renderer
)

target_link_libraries(Game PRIVATE
        project_sanitizers
        project_warnings
)
