#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <raylib.h>
#include <vector>

class MapRenderer {
public:
    bool LoadMap(const std::string& path);
    void Draw();
    void Unload();

    Vector2 GetMapSize() const;
    bool IsTileSolid( int x, int y) const;
    Rectangle GetMapBounds() const;

private:
    tmx::Map map;
    Texture2D tilesetTexture;
    std::string tilesetImagePath;

    std::vector<std::vector<bool>> collisionMap;
    tmx::Vector2u tileSize;
    tmx::Vector2u mapSize;

    void LoadTileset();
    void DrawTileLayer(const tmx::TileLayer& layer);
    void BuildCollisionMap();
};