#include <raylib.h>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "basic window");

    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        BeginDrawing();

        ClearBackground(WHITE);

        DrawCircle(400, 225, 25, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
