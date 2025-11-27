cmake_minimum_required(VERSION 3.27)
project(c__ LANGUAGES CXX)

set(CMAKE_TOOLCHAIN_FILE "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib REQUIRED)
find_package(glfw3 REQUIRED)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(c__ src/main.cpp
        src/Window.cpp
        src/Window.h
        src/Player2D.cpp
        src/Player2D.h
)

target_link_libraries(c__ PRIVATE
        raylib
        glfw
)

