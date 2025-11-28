#include "Player2D.h"
#include <cmath>
#include <iostream>
#include <raylib.h>

void Player2D::LoadTextures() {
    spriteSheet = LoadTexture("../assets/textures/character_spritesheet.png");
    // grass = LoadTexture("../assets/textures/grass.png");

    frameRec = {
        0.0f,
        0.0f,
        (float)spriteWidth,
        (float)spriteHeight
    };
}

void Player2D::UnloadTextures() {
    UnloadTexture(spriteSheet);
    UnloadTexture(grass);
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

}

void Player2D::Draw() {
    // REMOVA ou COMENTE este loop que tenta desenhar grama não carregada:
    /*
    for (int y = 0; y < GetScreenHeight(); y += grass.height) {
        for (int x = 0; x < GetScreenWidth(); x += grass.width) {
            DrawTexture(grass, x, y, WHITE);
        }
    }
    */

    float scale = 3.0f;

    Rectangle dest = {
        position.x,
        position.y,
        frameRec.width * scale,
        frameRec.height * scale
    };

    Vector2 origin = {0, 0};

    DrawTexturePro(
        spriteSheet,
        frameRec,
        dest,
        origin,
        0.0f,
        WHITE
    );

    // DEBUG: Direção (opcional)
    const char* dirNames[] = {"BAIXO", "ESQUERDA", "DIREITA", "CIMA"};
    DrawText(dirNames[currentDirection], (int)position.x + 20, (int)position.y - 20, 20, YELLOW);
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
    }
}
