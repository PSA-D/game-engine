cmake_minimum_required(VERSION 4.3)

project(ProjectSystemArchitecture
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENGINE_BUILD_TESTS "Build tests" ON)
option(ENGINE_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" ON)

add_library(project_warnings INTERFACE)

if(NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "Environment variable VCPKG_ROOT is not set. Install vcpkg and set VCPKG_ROOT (see DEVELOPMENT_SETUP.md, section 5).")
endif()

if(MSVC)
    target_compile_options(project_warnings INTERFACE /W4 /WX)
else()
    target_compile_options(project_warnings INTERFACE -Wall -Wextra -Wpedantic -Werror)
endif()

add_library(project_sanitizers INTERFACE)

if(ENGINE_ENABLE_SANITIZERS AND NOT MSVC AND NOT MINGW)
    target_compile_options(project_sanitizers INTERFACE -fsanitize=address,undefined)
    target_link_options(project_sanitizers INTERFACE -fsanitize=address,undefined)
endif()

add_subdirectory(engine)
add_subdirectory(game)

if(ENGINE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
