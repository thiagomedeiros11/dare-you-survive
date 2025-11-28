#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <raylib.h>

class MapRenderer {
public:
    bool LoadMap(const std::string& path);
    void Draw();
    void Unload();

private:
    tmx::Map map;
    Texture2D tilesetTexture;
    std::string tilesetImagePath;

    void LoadTileset();
    void DrawTileLayer(const tmx::TileLayer& layer);
};