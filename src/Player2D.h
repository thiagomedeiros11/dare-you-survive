#pragma once
#include <raylib.h>

class Player2D {
public:
    Vector2 position = {400.0f, 300.0f};
    Vector2 velocity = {0.0f, 0.0f};
    float speed = 3.0f;

    Texture2D characterSprite;

    Rectangle frameRec;
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 8;

    int spriteWidth = 32;
    int spriteHeight = 32;
    int framesCount = 3;
    int directionsCount = 4;

    bool isMoving = false;
    int currentDirection = 0;

    Rectangle GetHitbox() const {
        float scale = 3.0f;
        return {
        position.x + 8 * scale,
        position.y + 16 * scale,
        (spriteWidth - 16) * scale,
        (spriteHeight - 8) * scale
        };
    }

    void LoadTextures();
    void UnloadTextures();
    void Update();
    void Draw();
    void Move(Vector2 direction);

    bool CheckCollision(const class MapRenderer& map) const;
};