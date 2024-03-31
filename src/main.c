#include <math.h>
#include <raylib.h>
#include <stdio.h>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    int circleX = 400;
    int circleY = 225;

    InitWindow(screenWidth, screenHeight, "basic window");

    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        BeginDrawing();

        ClearBackground(WHITE);

        DrawCircle(circleX + 125 * sin(GetTime()), circleY + 125 * cos(GetTime()), 25, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
