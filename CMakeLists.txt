cmake_minimum_required(VERSION 3.25)

project(NewTradingSystem
    VERSION 0.1.0
    LANGUAGES CXX
)

# ============================================================
# C++ Standard
# ============================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# Include Directory
# ============================================================
target_include_directories(NewTradingSystemCore
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# ============================================================
# Source Discovery
# ============================================================
file(GLOB_RECURSE PROJECT_SOURCES
    ${PROJECT_SOURCE_DIR}/src/trading/*.cpp
)

file(GLOB_RECURSE PROJECT_HEADERS
    ${PROJECT_SOURCE_DIR}/include/trading/*.hpp
)

# ============================================================
# Trading Core Library
# ============================================================
add_library(NewTradingSystemCore
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

# ============================================================
# Main Application
# ============================================================
add_executable(NewTradingSystem
    src/main.cpp
)

target_link_libraries(NewTradingSystem
    PRIVATE
    NewTradingSystemCore
)

# ============================================================
# Compiler Options
# ============================================================
if(MSVC)
    target_compile_options(NewTradingSystemCore PRIVATE /W4 /permissive-)
else()
    target_compile_options(NewTradingSystemCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ============================================================
# Tests Placeholder
# ============================================================
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
endif()
