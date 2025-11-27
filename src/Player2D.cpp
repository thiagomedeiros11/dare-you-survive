#include "Player2D.h"
#include <cmath>
#include <iostream>
#include <raylib.h>

void Player2D::LoadTextures() {
    spriteSheet = LoadTexture("../assets/textures/character_spritesheet.png");
    frameRec = {
        0.0f,
        0.0f,
        (float)spriteWidth,
        (float)spriteHeight
    };
    std::cout << "Spritesheet carregada (esperado 3x4 frames): spriteWidth=" << spriteWidth
              << " spriteHeight=" << spriteHeight << " framesCount=" << framesCount
              << " directionsCount=" << directionsCount << std::endl;
}

void Player2D::UnloadTextures() {
    UnloadTexture(spriteSheet);
}

void Player2D::Update() {
    bool moving = (velocity.x != 0 || velocity.y != 0);

    if (moving) {
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

    position.x += velocity.x;
    position.y += velocity.y;

    if (position.x < 0) position.x = 0;
    if (position.x > GetScreenWidth() - spriteWidth) position.x = GetScreenWidth() - spriteWidth;
    if (position.y < 0) position.y = 0;
    if (position.y > GetScreenHeight() - spriteHeight) position.y = GetScreenHeight() - spriteHeight;
}

void Player2D::Draw() {
    DrawTextureRec(spriteSheet, frameRec, position, WHITE);

    const char* dirNames[] = {"BAIXO", "ESQUERDA", "DIREITA", "CIMA"};
    DrawText(dirNames[currentDirection], (int)position.x - 10, (int)position.y - 40, 15, YELLOW);

    std::string frameInfo = "F:" + std::to_string(currentFrame);
    DrawText(frameInfo.c_str(), (int)position.x - 10, (int)position.y - 25, 15, GREEN);

    std::string rectInfo = "XY:" + std::to_string((int)frameRec.x) + "," + std::to_string((int)frameRec.y);
    DrawText(rectInfo.c_str(), (int)position.x - 10, (int)position.y - 10, 15, ORANGE);
}

void Player2D::Move(Vector2 direction) {
    if (direction.x == 0 && direction.y == 0) {
        velocity = {0,0};
        isMoving = false;
        return;
    }

    float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.0f) {
        direction.x /= length;
        direction.y /= length;
    }

    velocity = { direction.x * speed, direction.y * speed };
    isMoving = true;

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
        std::cout << "Move debug: dir=(" << direction.x << "," << direction.y << ") -> currentDirection=" << currentDirection
                  << " frameRec=(" << (int)frameRec.x << "," << (int)frameRec.y << ")\n";
    }
}
