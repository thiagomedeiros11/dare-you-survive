#include <iostream>
#include "Window.h"
#include "Player2D.h"
#include "MapRenderer.h"

void Window::startWindow() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Dare You Survive - 2D");

    Font vt323 = LoadFont("../assets/fonts/VT323-Regular.ttf");

    Player2D player;
    player.position = {100, 100};
    player.LoadTextures();

    MapRenderer mapRenderer;
    if (!mapRenderer.LoadMap("../assets/map/map.tmx")) {
        std::cout << "Failed to load map!" << std::endl;
    }

    SetTargetFPS(60);

    Camera2D camera = {0};
    camera.target = player.position;
    camera.offset = { screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

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

        camera.target = player.position;

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        mapRenderer.Draw();

        player.Draw();

        EndMode2D();

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

        EndDrawing();
    }

    player.UnloadTextures();
    mapRenderer.Unload();
    CloseWindow();
}