# Minimum version of cmake
cmake_minimum_required(VERSION 3.15)
project(StealthGame LANGUAGES CXX)

# Requires C++17 or throws an error
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enables FetchContent to get an external library like libtcod
include(FetchContent)

# Declares the libtcod library and specific version of 1.24.0 (for stability)
FetchContent_Declare(
    libtcod
    GIT_REPOSITORY https://github.com/libtcod/libtcod.git
    GIT_TAG 1.24.0
)
FetchContent_MakeAvailable(libtcod)

# Finds all .cpp files in src directory, adds them to sources
# CONFIUGRE_DEPENDS = Runs if new files are added to the src directory
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Adds executable to run with ./build/game
# game has all the sources inside
add_executable(game ${SOURCES})

# Links libtcod to the executable
target_include_directories(game PRIVATE include)

# Links libtcod to the executable
target_link_libraries(game PRIVATE tcod)

# Sets the compile options for the executable
# MSVC = Microsoft Visual C++
# /W4 = All warnings enabled
# /WX = Treat warnings as errors
if (MSVC)
    target_compile_options(game PRIVATE /W4 /WX)
else()
    target_compile_options(game PRIVATE -Wall -Wextra -pedantic -Werror)
endif()