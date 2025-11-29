#include "MapManager.h"
#include "Player2D.h"
#include <iostream>

MapManager::MapManager() : currentMap(nullptr) {}

MapManager::~MapManager() {
    Unload();
}

bool MapManager::LoadInitialMap(const std::string& path) {
    maps.clear();
    transitions.clear();

    MapRenderer newMap;
    if (newMap.LoadMap(path)) {
        maps.push_back(std::move(newMap));
        currentMap = &maps.back();
        SetupTransitions();
        return true;
    }
    return false;
}

void MapManager::LoadMap(const std::string& path) {
    for (auto& existingMap : maps) {
        if (existingMap.GetFilePath() == path) {
            currentMap = &existingMap;
            SetupTransitions();
            return;
        }
    }

    MapRenderer newMap;
    if (newMap.LoadMap(path)) {
        maps.push_back(std::move(newMap));
        currentMap = &maps.back();
        SetupTransitions();
    }
}

void MapManager::SetupTransitions() {
    transitions.clear();

    if (!currentMap) return;

    Vector2 mapSize = currentMap->GetMapSize();

    if (currentMap->GetFilePath().find("map.tmx") != std::string::npos) {
        transitions.push_back({
            {mapSize.x - 50, 0, 50, mapSize.y}, // Área de trigger
            "../assets/map/map2.tmx",           // Próximo mapa
            {771, 228}                 // Posição de spawn no map1
        });
    }
    else if (currentMap->GetFilePath().find("map2.tmx") != std::string::npos) {
        // Quando player chega na borda esquerda, volta para map1
        transitions.push_back({
            {0, 0, 50, mapSize.y},              // Área de trigger
            "../assets/map/map.tmx",            // Mapa anterior
            {100, 250}    // Posição de spawn no map2
        });
    }
}

void MapManager::Update(Player2D& player) {
    if (!currentMap) return;

    Rectangle playerHitbox = player.GetHitbox();

    for (const auto& transition : transitions) {
        if (CheckCollisionRecs(playerHitbox, transition.triggerArea)) {
            LoadMap(transition.targetMap);
            player.position = transition.spawnPosition;
            break;
        }
    }
}

void MapManager::Draw() {
    if (currentMap) {
        currentMap->Draw();
    }
}

bool MapManager::CheckCollision(const Player2D& player) const {
    if (!currentMap) return false;
    return player.CheckCollision(*currentMap);
}

void MapManager::Unload() {
    for (auto& map : maps) {
        map.Unload();
    }
    maps.clear();
    currentMap = nullptr;
}