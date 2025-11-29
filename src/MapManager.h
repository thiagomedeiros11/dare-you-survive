#pragma once

#include "MapRenderer.h"
#include <vector>
#include <string>
#include <raylib.h>

class Player2D;

struct MapTransition {
    Rectangle triggerArea;
    std::string targetMap;
    Vector2 spawnPosition;
};

class MapManager {
public:
    MapManager();
    ~MapManager();

    bool LoadInitialMap(const std::string& path);
    void Update(Player2D& player);
    void Draw();
    void Unload();

    bool CheckCollision(const Player2D& player) const;

private:
    std::vector<MapRenderer> maps;
    std::vector<MapTransition> transitions;
    MapRenderer* currentMap;

    void LoadMap(const std::string& path);
    void SetupTransitions();
};