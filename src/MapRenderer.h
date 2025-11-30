#pragma once

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <raylib.h>
#include <vector>

class MapRenderer {
public:
    MapRenderer() = default;

    MapRenderer(const MapRenderer& other);

    MapRenderer& operator=(const MapRenderer& other);

    bool LoadMap(const std::string& path);
    void Draw();
    void Unload();

    Vector2 GetMapSize() const;
    bool IsTileSolid( int x, int y) const;
    Rectangle GetMapBounds() const;
    tmx::Vector2u GetTileSize() const { return tileSize; }
    std::string GetFilePath() const { return filePath; }
    float GetScale() const { return scale; }
    void SetScale(float newScale) { scale = newScale;};


private:
    tmx::Map map;
    Texture2D tilesetTexture;
    std::string tilesetImagePath;
    std::string filePath;

    std::vector<std::vector<bool>> collisionMap;
    tmx::Vector2u tileSize;
    tmx::Vector2u mapSize;
    float scale = 3.0f;

    void LoadTileset();
    void DrawTileLayer(const tmx::TileLayer& layer);
    void BuildCollisionMap();

    void CopyFrom(const MapRenderer& other);
};