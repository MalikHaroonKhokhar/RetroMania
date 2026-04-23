cmake_minimum_required(VERSION 3.16)
project(Forge2D VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch dependencies
include(FetchContent)

# nlohmann_json
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# SDL2
FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.2
)
set(SDL_SHARED ON CACHE BOOL "" FORCE)
set(SDL_STATIC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2)

# SDL2_mixer
FetchContent_Declare(
    SDL2_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-2.8.0
)
set(SDL2MIXER_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2MIXER_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2_mixer)

# SDL2_ttf
FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.22.0
)
set(SDL2TTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2TTF_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2_ttf)

# SDL2_image
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.2
)
set(SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE)
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(SDL2_image)

# Add Engine library
file(GLOB_RECURSE ENGINE_SOURCES
    "engine/*.cpp"
)
file(GLOB_RECURSE ENGINE_HEADERS
    "engine/*.h"
)

add_library(Forge2D STATIC ${ENGINE_SOURCES})
# Add a dummy file if no sources yet to avoid error
if(NOT ENGINE_SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    target_sources(Forge2D PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

target_include_directories(Forge2D PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
)

target_link_libraries(Forge2D PUBLIC
    nlohmann_json::nlohmann_json
    SDL2::SDL2
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
)

# Demo executable
file(GLOB_RECURSE DEMO_SOURCES "demo/*.cpp")
add_executable(Demo ${DEMO_SOURCES})
if(NOT DEMO_SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/demo_dummy.cpp "int main() { return 0; }")
    target_sources(Demo PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/demo_dummy.cpp)
endif()
target_link_libraries(Demo PRIVATE Forge2D)

# Tests
enable_testing()
include(GoogleTest)

# Unit Tests
file(GLOB_RECURSE UNIT_TEST_SOURCES "tests/unit/*.cpp")
if(UNIT_TEST_SOURCES)
    add_executable(UnitTests ${UNIT_TEST_SOURCES})
    target_link_libraries(UnitTests PRIVATE Forge2D GTest::gtest_main)
    gtest_discover_tests(UnitTests)
endif()

# Integration Tests
file(GLOB_RECURSE INTEGRATION_TEST_SOURCES "tests/integration/*.cpp")
if(INTEGRATION_TEST_SOURCES)
    add_executable(IntegrationTests ${INTEGRATION_TEST_SOURCES})
    target_link_libraries(IntegrationTests PRIVATE Forge2D GTest::gtest_main)
    gtest_discover_tests(IntegrationTests)
endif()

# E2E Tests
file(GLOB_RECURSE E2E_TEST_SOURCES "tests/e2e/*.cpp")
if(E2E_TEST_SOURCES)
    add_executable(E2ETests ${E2E_TEST_SOURCES})
    target_link_libraries(E2ETests PRIVATE Forge2D GTest::gtest_main)
    gtest_discover_tests(E2ETests)
endif()
