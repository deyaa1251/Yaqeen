cmake_minimum_required(VERSION 3.14)

# Set policies for newer CMake versions
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)  # Set timestamps to extraction time
endif()

# Allow older CMake versions in subprojects (for compatibility with dependencies)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(yaqeen VERSION 1.0.0 LANGUAGES CXX)

# C++17 required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Release optimizations
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

# FetchContent for dependencies
include(FetchContent)

# Set minimum version for subdirectories to avoid compatibility issues
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# CLI11 - Modern command line parser (updated to v2.4.2 for CMake 4.x compatibility)
FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG v2.4.2
    GIT_SHALLOW TRUE
)

# FTXUI - Terminal UI library
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG v5.0.0
    GIT_SHALLOW TRUE
)

# nlohmann_json - JSON library
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# md4c - Markdown parser
FetchContent_Declare(
    md4c
    GIT_REPOSITORY https://github.com/mity/md4c
    GIT_TAG release-0.4.8
    GIT_SHALLOW TRUE
)

message(STATUS "Fetching dependencies...")
FetchContent_MakeAvailable(CLI11 ftxui json md4c)

# Main executable sources
set(YAQEEN_SOURCES
    src/main.cpp
    src/core/parser.cpp
    src/core/generator.cpp
    src/core/template_manager.cpp
    src/ui/animations.cpp
    src/ui/progress.cpp
    src/ui/theme.cpp
    src/utils/logger.cpp
    src/utils/error.cpp
    src/utils/validators.cpp
)

# Create executable
add_executable(yaqeen ${YAQEEN_SOURCES})

# Include directories
target_include_directories(yaqeen PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(yaqeen PRIVATE
    CLI11::CLI11
    ftxui::screen
    ftxui::dom
    ftxui::component
    nlohmann_json::nlohmann_json
    md4c
)

# Platform-specific libraries
if(UNIX AND NOT APPLE)
    target_link_libraries(yaqeen PRIVATE pthread)
endif()

# Install targets
install(TARGETS yaqeen DESTINATION bin)
install(DIRECTORY templates DESTINATION share/yaqeen)

# Tests (optional)
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2
        GIT_TAG v3.4.0
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(yaqeen_tests
        tests/test_parser.cpp
        tests/test_generator.cpp
        tests/test_templates.cpp
        src/core/parser.cpp
        src/core/generator.cpp
        src/core/template_manager.cpp
        src/utils/logger.cpp
        src/utils/error.cpp
        src/utils/validators.cpp
    )

    target_include_directories(yaqeen_tests PRIVATE include)
    target_link_libraries(yaqeen_tests PRIVATE
        Catch2::Catch2WithMain
        nlohmann_json::nlohmann_json
        md4c
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(yaqeen_tests)
endif()

# Print configuration
message(STATUS "")
message(STATUS "Yaqeen Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Build Tests: ${BUILD_TESTS}")
message(STATUS "")
