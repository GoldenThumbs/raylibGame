cmake_minimum_required(VERSION 3.0)
project(game C)

set(CMAKE_C_STANDARD 99)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ENV{RAYLIB_PATH} C:/Dev/raylib/src)

add_library(raylib STATIC IMPORTED ${RAYLIB_PATH})
add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} raylib)