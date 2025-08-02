#include <iostream>
#include <raylib.h>
#include <cstdlib>
#include <ctime>
using namespace std;

int player1Score = 0;
int player2Score = 0;

class Ball {
public:
    float x, y;
    float speedX, speedY;
    int radius;

    void draw() {
        DrawCircle(x, y, radius, WHITE);
    }

    void update() {
        x += speedX;
        y += speedY;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speedY *= -1;
        }

        if (x + radius >= GetScreenWidth()) {
            player1Score++;
            reset();
        }
        else if (x - radius <= 0) {
            player2Score++;
            reset();
        }

        if (abs(speedX) < 20) speedX += (speedX > 0 ? 0.001f : -0.001f);
        if (abs(speedY) < 20) speedY += (speedY > 0 ? 0.001f : -0.001f);
    }

    void reset() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;
        int dirX = (rand() % 2 == 0) ? 1 : -1;
        int dirY = (rand() % 2 == 0) ? 1 : -1;
        speedX = 7 * dirX;
        speedY = 7 * dirY;
    }
};

class Paddle {
public:
    float x, y;
    float width, height;
    int speed;
    int upKey, downKey;

    void draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void update() {
        if (IsKeyDown(upKey) && y > 0) y -= speed;
        if (IsKeyDown(downKey) && y + height < GetScreenHeight()) y += speed;

        if (y < 0) y = 0;
        if (y + height > GetScreenHeight()) y = GetScreenHeight() - height;
    }
};

int main() {
    srand(time(0));
    const int screenWidth = 1200;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "ABKR Ping Pong");
    SetTargetFPS(60);

    // Game objects
    Ball ball;
    ball.radius = 20;
    ball.reset();

    Paddle paddle1, paddle2;
    paddle1 = {5, screenHeight / 2 - 60, 25, 100, 10, KEY_W, KEY_S};
    paddle2 = {screenWidth - 30, screenHeight / 2 - 60, 25, 100, 10, KEY_UP, KEY_DOWN};

    // -------------------
    // 🎬 Welcome Screen
    // -------------------
    bool gameStarted = false;
    float blinkTimer = 0;
    while (!gameStarted && !WindowShouldClose()) {
        blinkTimer += GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Welcome to ABKR PING PONG", screenWidth / 2 - MeasureText("Welcome to ABKR PING PONG", 50) / 2, screenHeight / 2 - 100, 50, WHITE);
        DrawText("Press [W] and [S] to move Player 1", screenWidth / 2 - MeasureText("Press [W] and [S] to move Player 1", 30) / 2, screenHeight / 2 - 40, 30, WHITE);
        DrawText("Press [UP] and [DOWN] to move Player 2", screenWidth / 2 - MeasureText("Press [UP] and [DOWN] to move Player 2", 30) / 2, screenHeight / 2 + 10, 30, WHITE);
        DrawText("A for Ayush, B for Bigyan, K for Kushal, R for Rujan", screenWidth / 2 - MeasureText("A for Ayush, B for Bigyan, K for Kushal, R for Rujan", 20) / 2, screenHeight / 2 + 80, 20, LIGHTGRAY);
        if (((int)(blinkTimer * 2)) % 2 == 0) {
            DrawText("Press [ENTER] to Start", screenWidth / 2 - MeasureText("Press [ENTER] to Start", 30) / 2, screenHeight / 2 + 150, 30, LIGHTGRAY);
        }
        if (IsKeyPressed(KEY_ENTER)) {
            gameStarted = true;
        }

        EndDrawing();
    }

   
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Update
        ball.update();
        paddle1.update();
        paddle2.update();

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
            Rectangle{paddle1.x, paddle1.y, paddle1.width, paddle1.height}) ||
            CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius,
            Rectangle{paddle2.x, paddle2.y, paddle2.width, paddle2.height})) {
            ball.speedX *= -1;
        }

        // Draw
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
        ball.draw();
        paddle1.draw();
        paddle2.draw();

        DrawText(TextFormat("Player 1 Score: %d", player1Score), 10, 10, 20, WHITE);
        DrawText(TextFormat("Player 2 Score: %d", player2Score), screenWidth - 200, 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
