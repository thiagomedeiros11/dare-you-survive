#include "Window.h"
#include <format>
#include "Player2D.h"
#include "MapManager.h"
#include <iostream>

void Window::startWindow() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "Dare You Survive - 2D Prototype");

    Font vt323 = LoadFont("../assets/fonts/VT323-Regular.ttf");

    Player2D player;
    float baseTileSize = 32.0f;
    float scaledTileSize = baseTileSize * 3.0f;
    player.position = {
        10 * scaledTileSize,
        10 * scaledTileSize
    };
    player.LoadTextures();

    MapManager mapManager;
    if (!mapManager.LoadInitialMap("../assets/map/map.tmx")) {
        std::cout << "Failed to load map!" << std::endl;
    }

    SetTargetFPS(60);

    Camera2D camera = {0};
    camera.target = player.position;
    camera.offset = { screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    bool canInput = true;
    float inputCooldown = 0.0f;

    while (!WindowShouldClose()) {
        int currentScreenWidth = GetScreenWidth();
        float deltaTime = GetFrameTime();

        if (inputCooldown > 0.0f) {
            inputCooldown -= deltaTime;
            canInput = false;
        } else {
            canInput = true;
        }

        player.velocity = {0, 0};
        player.isMoving = false;

        Vector2 direction = {0, 0};

        if (canInput && !player.isMovingToTile) {
            bool rightPressed = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);
            bool leftPressed = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
            bool downPressed = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
            bool upPressed = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);

            if (rightPressed && !leftPressed) {
                direction.x = 1;
            } else if (leftPressed && !rightPressed) {
                direction.x = -1;
            } else if (downPressed && !upPressed) {
                direction.y = 1;
            } else if (upPressed && !downPressed) {
                direction.y = -1;
            }

            if (direction.x != 0 || direction.y != 0) {
                player.MoveToTile(direction);
                inputCooldown = 0.1f;
            }
        }

        Vector2 previousPosition = player.position;

        player.Update();

        mapManager.Update(player);

        if (mapManager.CheckCollision(player)) {
            player.position = previousPosition;
            player.targetPosition = previousPosition;
            player.isMovingToTile = false;
            player.isMoving = false;
        }

        camera.target = player.position;

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);

        mapManager.Draw();
        player.Draw();

        EndMode2D();

        std::string fps = "FPS: " + std::to_string(GetFPS());
        DrawTextEx(vt323, fps.c_str(), {10, 10}, 32, 1, WHITE);

        const char* title = "Dare you survive - 2D Prototype";
        Vector2 titleSize = MeasureTextEx(vt323, title, 32, 1);
        float titleX = (currentScreenWidth - titleSize.x) / 2;
        DrawTextEx(vt323, title, {titleX, 10}, 32, 1, WHITE);

        std::string playerPos = "Position: X " +
                        std::to_string((int)player.position.x) +
                        ", Y " +
                        std::to_string((int)player.position.y);
        DrawTextEx(vt323, playerPos.c_str(), {10, 50}, 24, 1, WHITE);

        Vector2 tilePos = player.GetTilePosition();
        std::string tileInfo = "Tile: " +
                        std::to_string((int)tilePos.x) +
                        ", " +
                        std::to_string((int)tilePos.y);
        DrawTextEx(vt323, tileInfo.c_str(), {10, 80}, 24, 1, WHITE);

        EndDrawing();
    }

    player.UnloadTextures();
    mapManager.Unload();
    CloseWindow();
}