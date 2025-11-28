#include "MapRenderer.h"
#include <iostream>
#include <raylib.h>

bool MapRenderer::LoadMap(const std::string& path) {
    if (map.load(path)) {
        const auto& layers = map.getLayers();

        for (const auto& layer : layers) {
            if (layer->getType() == tmx::Layer::Type::Tile) {
                const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                const auto& tiles = tileLayer.getTiles();
            }
        }
        LoadTileset();
        return true;
    }
    std::cout << "Failed to load map: " << path << std::endl;
    return false;
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

void MapRenderer::Unload() {
    if (tilesetTexture.id > 0) {
        UnloadTexture(tilesetTexture);
    }
}