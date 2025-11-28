#include "MapRenderer.h"
#include <iostream>
#include <raylib.h>

bool MapRenderer::LoadMap(const std::string& path) {
    if (map.load(path)) {
        std::cout << "Map loaded successfully: " << path << std::endl;
        std::cout << "Map size: " << map.getBounds().width << "x" << map.getBounds().height << std::endl;
        std::cout << "Tile size: " << map.getTileSize().x << "x" << map.getTileSize().y << std::endl;

        const auto& layers = map.getLayers();
        std::cout << "Number of layers: " << layers.size() << std::endl;

        for (const auto& layer : layers) {
            if (layer->getType() == tmx::Layer::Type::Tile) {
                const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
                const auto& tiles = tileLayer.getTiles();
                std::cout << "Tile layer '" << layer->getName() << "' has " << tiles.size() << " tiles" << std::endl;

                // Debug dos primeiros tiles
                std::cout << "First row tiles: ";
                for (int i = 0; i < 30 && i < tiles.size(); i++) {
                    std::cout << tiles[i].ID << " ";
                }
                std::cout << std::endl;
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
        std::cout << "Loading tileset: " << fullPath << std::endl;

        tilesetTexture = LoadTexture(fullPath.c_str());
        if (tilesetTexture.id > 0) {
            std::cout << "Tileset loaded successfully" << std::endl;
            std::cout << "First GID: " << tileset.getFirstGID() << std::endl;
            std::cout << "Tile count: " << tileset.getTileCount() << std::endl;
            std::cout << "Columns: " << tileset.getColumnCount() << std::endl;
        }
    }
}

void MapRenderer::DrawTileLayer(const tmx::TileLayer& layer) {
    const auto& tiles = layer.getTiles();
    const auto tileSize = map.getTileSize();
    const auto& tileset = map.getTilesets()[0];

    // CORREÇÃO: Use as dimensões corretas do layer
    auto layerSize = layer.getSize();
    std::cout << "Drawing layer - Size: " << layerSize.x << "x" << layerSize.y
              << ", Tiles: " << tiles.size() << std::endl;

    // Loop correto para mapa 30x20
    for (std::size_t y = 0; y < layerSize.y; ++y) {
        for (std::size_t x = 0; x < layerSize.x; ++x) {
            std::size_t index = y * layerSize.x + x;

            if (index < tiles.size()) {
                const auto& tile = tiles[index];

                // Tile vazio é 0, mas no seu mapa os IDs começam em 53
                if (tile.ID != 0) {
                    float posX = static_cast<float>(x * tileSize.x);
                    float posY = static_cast<float>(y * tileSize.y);

                    // CORREÇÃO IMPORTANTE: No Tiled com firstgid="1",
                    // o tile.ID já é o índice correto no tileset
                    // Não subtraia o firstGID!
                    std::size_t tileID = tile.ID - 1; // Apenas subtrai 1 porque Tiled começa em 1

                    // Verifica se é um tile válido (0-255 para tileset de 256 tiles)
                    if (tileID < 256) { // tileset.getTileCount() deve ser 256
                        std::size_t tilesetColumns = 16; // do seu tileset.tsx
                        std::size_t tilesetX = (tileID % tilesetColumns) * tileSize.x;
                        std::size_t tilesetY = (tileID / tilesetColumns) * tileSize.y;

                        Rectangle sourceRect = {
                            static_cast<float>(tilesetX),
                            static_cast<float>(tilesetY),
                            static_cast<float>(tileSize.x),
                            static_cast<float>(tileSize.y)
                        };

                        // Desenha o tile
                        DrawTextureRec(tilesetTexture, sourceRect, {posX, posY}, WHITE);

                        // DEBUG: Desenha o ID do tile (opcional)
                        // DrawText(TextFormat("%d", tile.ID), posX + 5, posY + 5, 8, RED);
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