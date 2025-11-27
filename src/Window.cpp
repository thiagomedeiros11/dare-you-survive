#include <iostream>
#include "Window.h"
#include "Player2D.h"

void Window::startWindow() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Dare You Survive - 2D");

    Font vt323 = LoadFont("../assets/fonts/VT323-Regular.ttf");
    Player2D player;
    player.LoadTextures();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        int currentScreenWidth = GetScreenWidth();

        player.velocity = {0, 0};
        player.isMoving = false;

        Vector2 direction = {0, 0};

        bool rightPressed = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
        bool leftPressed = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);

        if (rightPressed && !leftPressed) {
            direction.x = 1;
        } else if (leftPressed && !rightPressed) {
            direction.x = -1;
        }

        bool downPressed = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
        bool upPressed = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);

        if (downPressed && !upPressed) {
            direction.y = 1;
        } else if (upPressed && !downPressed) {
            direction.y = -1;
        }

        if (direction.x != 0 || direction.y != 0) {
            player.Move(direction);
        }

        player.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        player.Draw();

        std::string fps = "FPS: " + std::to_string(GetFPS());
        DrawTextEx(vt323, fps.c_str(), {10, 10}, 32, 1, WHITE);

        const char* title = "Dare you survive - 2D Prototype";
        Vector2 titleSize = MeasureTextEx(vt323, title, 32, 1);
        float titleX = (currentScreenWidth - titleSize.x) / 2;
        DrawTextEx(vt323, title, {titleX, 10}, 32, 1, WHITE);

        std::string playerPos = "Position: " +
                               std::to_string((int)player.position.x) + ", " +
                               std::to_string((int)player.position.y);
        DrawTextEx(vt323, playerPos.c_str(), {10, 50}, 24, 1, WHITE);

        std::string inputDebug = "Input: ";
        if (rightPressed) inputDebug += "RIGHT ";
        if (leftPressed) inputDebug += "LEFT ";
        if (downPressed) inputDebug += "DOWN ";
        if (upPressed) inputDebug += "UP ";

        EndDrawing();
    }

    player.UnloadTextures();
    CloseWindow();
}