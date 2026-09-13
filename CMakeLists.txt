cmake_minimum_required(VERSION 3.25)

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

add_library(NewTradingSystemCore
    ${CORE_SOURCES}
)

target_include_directories(NewTradingSystemCore
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
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
