#include "Player2D.h"
#include "MapRenderer.h"
#include <raylib.h>
#include <cmath>
#include <iostream>

void Player2D::LoadTextures() {
    characterSprite = LoadTexture("../assets/textures/character_spritesheet.png");
    frameRec = {
        0.0f,
        0.0f,
        (float)spriteWidth,
        (float)spriteHeight
    };
    targetPosition = position;
}

void Player2D::UnloadTextures() {
    UnloadTexture(characterSprite);
}

void Player2D::Update() {
    if (isMovingToTile) {
        Vector2 direction = {
            targetPosition.x - position.x,
            targetPosition.y - position.y
        };

        float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);

        if (distance > 2.0f) {
            direction.x /= distance;
            direction.y /= distance;

            position.x += direction.x * movementSpeed * GetFrameTime();
            position.y += direction.y * movementSpeed * GetFrameTime();

            isMoving = true;
        } else {
            position = targetPosition;
            isMovingToTile = false;
            isMoving = false;
            currentFrame = 0;
            framesCounter = 0;
        }
    }

    if (isMoving) {
        framesCounter++;
        if (framesCounter >= (60 / framesSpeed)) {
            framesCounter = 0;
            currentFrame++;
            if (currentFrame >= framesCount) currentFrame = 0;
        }
    } else {
        currentFrame = 0;
        framesCounter = 0;
    }

    frameRec.x = (float)currentFrame * (float)spriteWidth;
    frameRec.y = (float)currentDirection * (float)spriteHeight;
}

void Player2D::Draw() {
    float scale = 3.0f;

    Rectangle dest = {
        position.x,
        position.y,
        frameRec.width * scale,
        frameRec.height * scale
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(
        characterSprite,
        frameRec,
        dest,
        origin,
        0.0f,
        WHITE
    );

    const char* dirNames[] = {"BAIXO", "ESQUERDA", "DIREITA", "CIMA"};
    DrawText(dirNames[currentDirection], (int)position.x + 20, (int)position.y - 20, 20, YELLOW);
}

Vector2 Player2D::GetTilePosition() const {
    float baseTileSize = 32.0f;
    float scaledTileSize = baseTileSize * 3.0f;

    return {
        std::round(position.x / scaledTileSize),
        std::round(position.y / scaledTileSize)
    };
}

bool Player2D::CanMoveToTile(Vector2 direction) const {
    Vector2 currentTile = GetTilePosition();
    Vector2 nextTile = {
        currentTile.x + direction.x,
        currentTile.y + direction.y
    };

    if (nextTile.x < 0 || nextTile.y < 0 || nextTile.x >= 30 || nextTile.y >= 20) {
        std::cout << "Cannot move - outside map bounds: (" << nextTile.x << ", " << nextTile.y << ")" << std::endl;
        return false;
    }

    float baseTileSize = 32.0f;
    float scaledTileSize = baseTileSize * 3.0f;

    Rectangle futureHitbox = {
        nextTile.x * scaledTileSize + 8 * 3.0f,
        nextTile.y * scaledTileSize + 16 * 3.0f,
        (spriteWidth - 16) * 3.0f,
        (spriteHeight - 8) * 3.0f
    };

    Rectangle mapBounds = {0, 0, 30 * scaledTileSize, 20 * scaledTileSize};
    if (futureHitbox.x < mapBounds.x || futureHitbox.y < mapBounds.y ||
        futureHitbox.x + futureHitbox.width > mapBounds.x + mapBounds.width ||
        futureHitbox.y + futureHitbox.height > mapBounds.y + mapBounds.height) {
        return false;
        }
    return true;
}

Rectangle Player2D::GetHitbox() const {
    float scale = 3.0f;
    float baseTileSize = 32.0f;

    return {
        position.x + 8 * scale,
        position.y + 16 * scale,
        (spriteWidth - 16) * scale,
        (spriteHeight - 8) * scale
    };
}

void Player2D::MoveToTile(Vector2 direction) {
    if (isMovingToTile) {
        return;
    }

    if (!CanMoveToTile(direction)) {
        return;
    }

    int newDirection = currentDirection;

    if (fabs(direction.x) > fabs(direction.y)) {
        if (direction.x > 0) newDirection = 2;
        else newDirection = 1;
    } else {
        if (direction.y > 0) newDirection = 0;
        else newDirection = 3;
    }

    if (newDirection != currentDirection) {
        currentDirection = newDirection;
        currentFrame = 0;
        framesCounter = 0;
    }

    float baseTileSize = 32.0f;
    float scaledTileSize = baseTileSize * 3.0f;

    Vector2 currentTile = GetTilePosition();
    Vector2 nextTile = {
        currentTile.x + direction.x,
        currentTile.y + direction.y
    };

    targetPosition = {
        nextTile.x * scaledTileSize,
        nextTile.y * scaledTileSize
    };

    isMovingToTile = true;
    isMoving = true;

    // Debug
    std::cout << "Moving from tile (" << currentTile.x << ", " << currentTile.y
              << ") to (" << nextTile.x << ", " << nextTile.y << ")"
              << " | Position: " << position.x << ", " << position.y
              << " | Target: " << targetPosition.x << ", " << targetPosition.y << std::endl;
}

bool Player2D::CheckCollision(const MapRenderer& map) const {
    Rectangle hitbox = GetHitbox();
    Rectangle mapBounds = map.GetMapBounds();

    if (hitbox.x < mapBounds.x || hitbox.y < mapBounds.y ||
        hitbox.x + hitbox.width > mapBounds.x + mapBounds.width ||
        hitbox.y + hitbox.height > mapBounds.y + mapBounds.height) {
        return true;
    }

    tmx::Vector2u tileSize = map.GetTileSize();
    float mapScale = map.GetScale();

    int leftTile = static_cast<int>(hitbox.x / (tileSize.x * mapScale));
    int rightTile = static_cast<int>((hitbox.x + hitbox.width) / (tileSize.x * mapScale));
    int topTile = static_cast<int>(hitbox.y / (tileSize.y * mapScale));
    int bottomTile = static_cast<int>((hitbox.y + hitbox.height) / (tileSize.y * mapScale));

    for (int y = topTile; y <= bottomTile; ++y) {
        for (int x = leftTile; x <= rightTile; ++x) {
            if (map.IsTileSolid(x, y)) {
                return true;
            }
        }
    }

    return false;
}