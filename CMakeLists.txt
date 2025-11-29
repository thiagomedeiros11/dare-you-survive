cmake_minimum_required(VERSION 3.27)
project(c__ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_TOOLCHAIN_FILE "/opt/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Encontrar pacotes com suporte CONFIG
find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

# Para tmxlite e zstd, usar find_library
find_library(TMXLITE_LIBRARY NAMES tmxlite)
find_library(ZSTD_LIBRARY NAMES zstd)

add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(c__
        src/main.cpp
        src/Window.cpp
        src/Window.h
        src/Player2D.cpp
        src/Player2D.h
        src/MapRenderer.cpp
        src/MapRenderer.h
        src/MapManager.cpp
        src/MapManager.h
)

target_link_libraries(c__ PRIVATE
        raylib
        glfw
        pugixml
        ${TMXLITE_LIBRARY}
        ZLIB::ZLIB
        ${ZSTD_LIBRARY}
)