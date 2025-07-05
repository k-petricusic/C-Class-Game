# Minimum version of cmake
cmake_minimum_required(VERSION 3.15)

# If the toolchain file is defined, set it to the default
if(DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_TOOLCHAIN_FILE}" CACHE STRING "Toolchain file")
endif()

project(StealthGame LANGUAGES CXX)

# Requires C++17 or throws an error
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Finds all .cpp files in src directory, adds them to sources
# CONFIUGRE_DEPENDS = Runs if new files are added to the src directory
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Adds executable to run with ./build/game
# game has all the sources inside
add_executable(game ${SOURCES})

# Copies the levels.txt file to the directory with the executable game.exe
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/levels.txt
        $<TARGET_FILE_DIR:game>
)

# Copies the terminal.png file to the directory with the executable game.exe
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/terminal8x8_gs_ro.png
        $<TARGET_FILE_DIR:game>
)

# Links libtcod to the executable
target_include_directories(game PRIVATE include)

# Links libtcod to the executable
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
find_package(libtcod CONFIG REQUIRED)
target_link_libraries(game PRIVATE SDL3::SDL3 libtcod::libtcod)

# Sets the compile options for the executable
# MSVC = Microsoft Visual C++
# /W4 = All warnings enabled
# /WX = Treat warnings as errors
if (MSVC)
    target_compile_options(game PRIVATE /W4 /WX)
else() # Enables all warnings and treats them as errors
    target_compile_options(game PRIVATE -Wall -Wextra -pedantic -Werror)
endif()