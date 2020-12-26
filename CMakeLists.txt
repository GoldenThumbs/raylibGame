cmake_minimum_required(VERSION 3.0)
project(game C)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_C_STANDARD 99)

# --- raylib options ---
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES "3.3" CACHE STRING "" FORCE)
# ----------------------
add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} raylib)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")