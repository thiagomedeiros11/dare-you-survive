# Dare You Survive - 2D Game Prototype


This project was created as a personal learning exercise to explore C++ and get hands-on experience with game development, 2D rendering, and TMX map loading.

The goal is to build a small game architecture from scratch and learn how different components interact.

# 🚀 Project Purpose

The main objective of this project is educational:

Learn modern C++20 with a low-level mindset

Understand pointers, memory layout, compilation, linking, and modular structure

Explore Raylib for rendering, textures, input, and window management

Load and interpret TMX maps created with the Tiled editor

Practice game architecture concepts

Experiment with performance and systems programming

Build a tiny foundation that could evolve into a 2D game framework

The project name "Dare You Survive" is simply a placeholder for experimentation.

# 🛠 Technologies Used

C++20

Dependencies (via vcpkg)

Defined in vcpkg.json:

raylib — 2D rendering, input, textures

glfw3 — window creation backend

tmxlite — TMX map loader

pugixml — XML parsing for TMX formats

zlib — compression support

zstd — optional map decompression

These are automatically resolved through vcpkg.

# 🔨 How to Build 

1. Install vcpkg (if not installed)  
   `git clone https://github.com/microsoft/vcpkg
   ./vcpkg/bootstrap-vcpkg.sh`


2. Install dependencies  
   `./vcpkg/vcpkg install raylib glfw3 tmxlite zlib zstd pugixml`  


3. Configure the project
   `cmake -B build -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake`


4. Build  
   `cmake --build build`

# 🗺️ TMX Map Workflow

This project uses Tiled to create the maps.

TMX files are parsed with tmxlite

XML parsing is handled by pugixml

Tile layers are converted into Raylib textures and drawn in order

Collision layers can be processed to restrict movement

Map switching is supported through the MapManager

This creates a simple but realistic 2D tile-based pipeline.

# 📄 License

This project is for learning purposes.
You may use or modify the code freely. A basic MIT License is recommended if needed.