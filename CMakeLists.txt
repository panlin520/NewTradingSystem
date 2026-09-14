cmake_minimum_required(VERSION 3.25)

# TEST COMMENT SYNC CHECK

project(NewTradingSystem
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

file(GLOB_RECURSE CORE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Remove application entry point from library
list(REMOVE_ITEM CORE_SOURCES
    ${PROJECT_SOURCE_DIR}/src/main.cpp
)

# ============================================================
# Local third party libraries
# ============================================================

# Build only the zstd library required by DBNReader.
# Disable bundled programs/tests/shared projects.
set(ZSTD_BUILD_PROGRAMS OFF CACHE BOOL "" FORCE)
set(ZSTD_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ZSTD_BUILD_SHARED OFF CACHE BOOL "" FORCE)

add_subdirectory(
    third_party/zstd/build/cmake
    EXCLUDE_FROM_ALL
)


add_library(NewTradingSystemCore
    ${CORE_SOURCES}
)


target_include_directories(NewTradingSystemCore
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/third_party/zstd/lib
)


# Link local zstd library for DBN .dbn.zst decoding

target_link_libraries(NewTradingSystemCore
    PRIVATE
        libzstd_static
)


# GoogleTest dependency for unit tests
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(tests)


add_executable(CMETradingSystem
    src/main.cpp
)


target_link_libraries(CMETradingSystem
    PRIVATE
        NewTradingSystemCore
)
