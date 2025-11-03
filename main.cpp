#include <raylib.h>
#include <thread>
#include <chrono>

int screenWidth = 1000;
int screenHeight = 600;

int player1Score = 0;
int player2Score = 0;
bool gameOver = false;

class Ball {
public:
    float x, y;
    float speedX, speedY;
    float radius;

    Ball() {
        x = screenWidth / 2.0f;
        y = screenHeight / 2.0f;
        speedX = 7;
        speedY = 7;
        radius = 20;
    }

    void draw() {
        DrawCircle((int)x, (int)y, radius, WHITE);
    }

    void update() {
        x += speedX;
        y += speedY;

        if (y + radius >= screenHeight || y - radius <= 0) {
            speedY *= -1;
        }

        if (x + radius >= screenWidth) {
            player1Score++;
            reset();
        }

        if (x - radius <= 0) {
            player2Score++;
            reset();
        }
    }

    void reset() {
        x = screenWidth / 2.0f;
        y = screenHeight / 2.0f;
        speedX *= -1;
        speedY = 7;
    }
};

class Paddle {
public:
    float x, y;
    float width, height;
    float speed;
    int upKey;
    int downKey;

    Paddle(float x, float y, int upKey, int downKey) {
        this->x = x;
        this->y = y;
        this->width = 25;
        this->height = 120;
        this->speed = 10;
        this->upKey = upKey;
        this->downKey = downKey;
    }

    void draw() {
        DrawRectangle((int)x, (int)y, (int)width, (int)height, WHITE);
    }

    void update() {
        if (IsKeyDown(upKey) && y > 0) {
            y -= speed;
        }
        if (IsKeyDown(downKey) && y + height < screenHeight) {
            y += speed;
        }
    }
};

void showCountdown() {
    for (int i = 3; i > 0; --i) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("%d", i), screenWidth / 2 - 20, screenHeight / 2 - 50, 80, WHITE);
        EndDrawing();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("GO!", screenWidth / 2 - 60, screenHeight / 2 - 50, 80, GREEN);
    EndDrawing();
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
}

int main() {
    InitWindow(screenWidth, screenHeight, "ABKR Ping Pong");
    SetTargetFPS(60);

  
    // Welcome screen
    while (!IsKeyPressed(KEY_ENTER) && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Welcome to ABKR Ping Pong", screenWidth / 2 - MeasureText("Welcome to ABKR Ping Pong", 30) / 2, screenHeight / 2 - 40, 30, WHITE);
        DrawText("Press [ENTER] to Start", screenWidth / 2 - MeasureText("Press [ENTER] to Start", 20) / 2, screenHeight / 2 + 20, 20, GREEN);
        EndDrawing();
    }

    // Instruction screen
    bool w = false, s = false, up = false, down = false;
    while (!w || !s || !up || !down) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Controls:", screenWidth / 2 - MeasureText("Controls:", 30) / 2, 100, 30, WHITE);
        DrawText("Player 1: W (Up), S (Down)", screenWidth / 2 - 200, 200, 20, WHITE);
        DrawText("Player 2: UP (Up), DOWN (Down)", screenWidth / 2 - 200, 240, 20, WHITE);
        DrawText("Press respective keys at least once to continue", screenWidth / 2 - 250, 300, 20, LIGHTGRAY);
        EndDrawing();

        if (IsKeyPressed(KEY_W)) w = true;
        if (IsKeyPressed(KEY_S)) s = true;
        if (IsKeyPressed(KEY_UP)) up = true;
        if (IsKeyPressed(KEY_DOWN)) down = true;
    }

    // Countdown animation
    showCountdown();

    Ball ball;
    Paddle paddle1(10, screenHeight / 2 - 60, KEY_W, KEY_S);
    Paddle paddle2(screenWidth - 35, screenHeight / 2 - 60, KEY_UP, KEY_DOWN);

    while (!WindowShouldClose()) {
        BeginDrawing();

        if (!gameOver) {
            ball.update(); // Update ball position
            ball.speedX += 0.001; // Gradually increase ball speed
            ball.speedY += 0.001;
            paddle1.update(); // Update paddle1 position
            paddle2.update(); // Update paddle2 position

            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle1.x, paddle1.y, paddle1.width, paddle1.height}) ||
                CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle2.x, paddle2.y, paddle2.width, paddle2.height})) {
                ball.speedX *= -1; // Reverse ball direction on paddle hit
            } 

            ClearBackground(BLACK);
            DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
            ball.draw();
            paddle1.draw();
            paddle2.draw();
            DrawText(TextFormat("Player 1 Score: %d", player1Score), 10, 10, 20, WHITE);
            DrawText(TextFormat("Player 2 Score: %d", player2Score), screenWidth - 200, 10, 20, WHITE);
            // Game over If any player reaches 20 points
            if (player1Score >= 20 || player2Score >= 20) {
                gameOver = true;
            }
        } else {
            ClearBackground(BLACK);
            const char* winnerText = player1Score >= 20 ? "Player 1 Wins!" : "Player 2 Wins!";
            DrawText(winnerText, screenWidth / 2 - MeasureText(winnerText, 40) / 2, screenHeight / 2 - 50, 40, GREEN);
            DrawText("Press [R] to Restart", screenWidth / 2 - MeasureText("Press [R] to Restart", 20) / 2, screenHeight / 2 + 10, 20, WHITE);
            DrawText("Press [ESC] to Exit", screenWidth / 2 - MeasureText("Press [ESC] to Exit", 20) / 2, screenHeight / 2 + 40, 20, WHITE);

            if (IsKeyPressed(KEY_R)) {
                player1Score = 0;
                player2Score = 0;
                ball.reset();
                gameOver = false;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}