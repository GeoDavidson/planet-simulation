#include <math.h>
#include <raylib.h>
#include <stdio.h>

double G = 6.674e-11;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
} Body;

int main() {
    const int winWidth = 800;
    const int winHeight = 450;

    Body body1 = {{winWidth / 3, winHeight / 2}, {0, 1.4}, {0, 0}, 500};
    Body body2 = {{winWidth / 2, winHeight / 2}, {0, 0}, {0, 0}, 5000000000000};

    double distance = 0;
    double force = 0;
    double angle = 0;
    double forceX = 0;
    double forceY = 0;

    int lastMousePosX = 0;
    int lastMousePosY = 0;
    float mouseOffsetX = -winWidth / 2;
    float mouseOffsetY = -winHeight / 2;

    float zoom = 1.0f;
    float zoomSpeed = 0.05f;

    InitWindow(winWidth, winHeight, "basic window");

    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        // zoom
        zoom += GetMouseWheelMove() * 0.05f;

        // move
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            mouseOffsetX += (GetMouseX() - lastMousePosX) / zoom;
            mouseOffsetY += (GetMouseY() - lastMousePosY) / zoom;
        }
        lastMousePosX = GetMousePosition().x;
        lastMousePosY = GetMousePosition().y;

        // body1
        distance = sqrt(pow((body2.position.x - body1.position.x), 2) + pow((body2.position.y - body1.position.y), 2));
        force = G * body1.mass * body2.mass / pow(distance, 2);
        angle = atan2(body2.position.y - body1.position.y, body2.position.x - body1.position.x);
        forceX = force * cos(angle);
        forceY = force * sin(angle);

        body1.acceleration.x = forceX / body1.mass;
        body1.acceleration.y = forceY / body1.mass;

        body1.velocity.x += body1.acceleration.x;
        body1.velocity.y += body1.acceleration.y;

        body1.position.x += body1.velocity.x;
        body1.position.y += body1.velocity.y;

        // body2
        distance = sqrt(pow((body1.position.x - body2.position.x), 2) + pow((body1.position.y - body2.position.y), 2));
        force = G * body2.mass * body1.mass / pow(distance, 2);
        angle = atan2(body1.position.y - body2.position.y, body1.position.x - body2.position.x);
        forceX = force * cos(angle);
        forceY = force * sin(angle);

        body2.acceleration.x = forceX / body2.mass;
        body2.acceleration.y = forceY / body2.mass;

        body2.velocity.x += body2.acceleration.x;
        body2.velocity.y += body2.acceleration.y;

        body2.position.x += body2.velocity.x;
        body2.position.y += body2.velocity.y;

        BeginDrawing();

        ClearBackground(WHITE);

        DrawCircle((body1.position.x + mouseOffsetX) * zoom + winWidth / 2, (body1.position.y + mouseOffsetY) * zoom + winHeight / 2, 20 * zoom, BLACK);
        DrawCircle((body2.position.x + mouseOffsetX) * zoom + winWidth / 2, (body2.position.y + mouseOffsetY) * zoom + winHeight / 2, 55 * zoom, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
