#include "MapRenderer.h"
#include <iostream>
#include <raylib.h>

MapRenderer::MapRenderer(const MapRenderer& other) {
    CopyFrom(other);
}

MapRenderer& MapRenderer::operator=(const MapRenderer& other) {
    if (this != &other) {
        Unload(); // Limpa recursos atuais
        CopyFrom(other);
    }
    return *this;
}

void MapRenderer::CopyFrom(const MapRenderer& other) {
    filePath = other.filePath;
    tilesetImagePath = other.tilesetImagePath;
    tileSize = other.tileSize;
    mapSize = other.mapSize;
    collisionMap = other.collisionMap;

    if (!filePath.empty()) {
        map.load(filePath);
    }

    if (!tilesetImagePath.empty()) {
        tilesetTexture = LoadTexture(tilesetImagePath.c_str());
    }
}

bool MapRenderer::LoadMap(const std::string& path) {
    filePath = path;
    if (map.load(path)) {
        tileSize = map.getTileSize();
        mapSize = map.getTileCount();

        BuildCollisionMap();
        LoadTileset();
        return true;
    }
    std::cout << "Failed to load map: " << path << std::endl;
    return false;
}

void MapRenderer::BuildCollisionMap() {
    collisionMap.resize(mapSize.y, std::vector<bool>(mapSize.x, false));

    const auto& layers = map.getLayers();
    for (const auto& layer : layers) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            const auto& tiles = tileLayer.getTiles();

            if (layer->getName().find("collision") != std::string::npos) {
                for (std::size_t y = 0; y < mapSize.y; ++y) {
                    for (std::size_t x = 0; x < mapSize.x; ++x) {
                        std::size_t index = y * mapSize.x + x;
                        if (index < tiles.size()) {
                            const auto& tile = tiles[index];
                            collisionMap[y][x] = (tile.ID != 0);
                        }
                    }
                }
            }
        }
    }
}

void MapRenderer::LoadTileset() {
    const auto& tilesets = map.getTilesets();
    if (!tilesets.empty()) {
        const auto& tileset = tilesets[0];
        tilesetImagePath = tileset.getImagePath();
        std::string fullPath = tilesetImagePath;
        tilesetTexture = LoadTexture(fullPath.c_str());
    }
}

void MapRenderer::DrawTileLayer(const tmx::TileLayer& layer) {
    const auto& tiles = layer.getTiles();
    const auto tileSize = map.getTileSize();

    auto layerSize = layer.getSize();

    for (std::size_t y = 0; y < layerSize.y; ++y) {
        for (std::size_t x = 0; x < layerSize.x; ++x) {
            std::size_t index = y * layerSize.x + x;

            if (index < tiles.size()) {
                const auto& tile = tiles[index];

                if (tile.ID != 0) {
                    float posX = static_cast<float>(x * tileSize.x);
                    float posY = static_cast<float>(y * tileSize.y);

                    std::size_t tileID = tile.ID - 1;

                    if (tileID < 256) {
                        std::size_t tilesetColumns = 16;
                        std::size_t tilesetX = (tileID % tilesetColumns) * tileSize.x;
                        std::size_t tilesetY = (tileID / tilesetColumns) * tileSize.y;

                        Rectangle sourceRect = {
                            static_cast<float>(tilesetX),
                            static_cast<float>(tilesetY),
                            static_cast<float>(tileSize.x),
                            static_cast<float>(tileSize.y)
                        };
                        DrawTextureRec(tilesetTexture, sourceRect, {posX, posY}, WHITE);
                    } else {
                        std::cout << "Invalid tileID: " << tileID << " from original ID: " << tile.ID << std::endl;
                    }
                }
            }
        }
    }
}

void MapRenderer::Draw() {
    const auto& layers = map.getLayers();
    for (const auto& layer : layers) {
        if (layer->getType() == tmx::Layer::Type::Tile) {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            DrawTileLayer(tileLayer);
        }
    }
}

Vector2 MapRenderer::GetMapSize() const {
    return {
        static_cast<float>(mapSize.x * tileSize.x),
        static_cast<float>(mapSize.y * tileSize.y)
    };
}

bool MapRenderer::IsTileSolid(int x, int y) const {
    if (x < 0 || y < 0 || x >= static_cast<int>(mapSize.x) || y >= static_cast<int>(mapSize.y)) {
        return true;
    }
    return collisionMap[y][x];
}

Rectangle MapRenderer::GetMapBounds() const {
    return {0, 0, GetMapSize().x, GetMapSize().y};
}

void MapRenderer::Unload() {
    if (tilesetTexture.id > 0) {
        UnloadTexture(tilesetTexture);
        tilesetTexture.id = 0;
    }
}