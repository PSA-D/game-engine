cmake_minimum_required(VERSION 3.25)

project(ProjectSystemArchitecture
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "Environment variable VCPKG_ROOT is not set. Install vcpkg and set VCPKG_ROOT (see DEVELOPMENT_SETUP.md, section 5).")
endif()

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

add_subdirectory(engine)
add_subdirectory(game)

option(ENGINE_BUILD_TESTS "Build tests" ON)

if(ENGINE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
