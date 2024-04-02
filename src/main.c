#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

double G = 6.674e-11;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
} Body;

Vector2 calculateForce(Body body1, Body body2) {
    Vector2 force = {};
    double distance = sqrt(pow((body2.position.x - body1.position.x), 2) + pow((body2.position.y - body1.position.y), 2));
    double forceMagnitude = G * body1.mass * body2.mass / pow(distance, 2);
    double angle = atan2(body2.position.y - body1.position.y, body2.position.x - body1.position.x);
    force.x = forceMagnitude * cos(angle);
    force.y = forceMagnitude * sin(angle);
    return force;
}

int main() {
    const int winWidth = 800;
    const int winHeight = 450;

    Body body1 = {{winWidth / 3, winHeight / 2}, {0, 1}, {0, 0}, 500000000000};
    Body body2 = {{winWidth / 2, winHeight / 2}, {0, 0}, {0, 0}, 5000000000000};
    Body body3 = {{winWidth / 1.5, winHeight / 2}, {0, -1}, {0, 0}, 5000000000000};

    Vector2 force = {0.0f, 0.0f};

    int lastMousePosX = 0;
    int lastMousePosY = 0;
    float mouseOffsetX = -winWidth / 2;
    float mouseOffsetY = -winHeight / 2;

    float zoom = 1.0f;
    float zoomSpeed = 0.05f;

    InitWindow(winWidth, winHeight, "basic window");

    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        zoom = Clamp(zoom + GetMouseWheelMove() * zoomSpeed, zoomSpeed, 10.0f);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            mouseOffsetX += (GetMouseX() - lastMousePosX) / zoom;
            mouseOffsetY += (GetMouseY() - lastMousePosY) / zoom;
        }
        lastMousePosX = GetMousePosition().x;
        lastMousePosY = GetMousePosition().y;

        force = calculateForce(body1, body2);

        body1.acceleration.x = force.x / body1.mass;
        body1.acceleration.y = force.y / body1.mass;

        body1.velocity.x += body1.acceleration.x;
        body1.velocity.y += body1.acceleration.y;

        body1.position.x += body1.velocity.x;
        body1.position.y += body1.velocity.y;

        force = calculateForce(body1, body3);

        body1.acceleration.x = force.x / body1.mass;
        body1.acceleration.y = force.y / body1.mass;

        body1.velocity.x += body1.acceleration.x;
        body1.velocity.y += body1.acceleration.y;

        body1.position.x += body1.velocity.x;
        body1.position.y += body1.velocity.y;

        force = calculateForce(body2, body1);

        body2.acceleration.x = force.x / body2.mass;
        body2.acceleration.y = force.y / body2.mass;

        body2.velocity.x += body2.acceleration.x;
        body2.velocity.y += body2.acceleration.y;

        body2.position.x += body2.velocity.x;
        body2.position.y += body2.velocity.y;

        force = calculateForce(body2, body3);

        body2.acceleration.x = force.x / body2.mass;
        body2.acceleration.y = force.y / body2.mass;

        body2.velocity.x += body2.acceleration.x;
        body2.velocity.y += body2.acceleration.y;

        body2.position.x += body2.velocity.x;
        body2.position.y += body2.velocity.y;

        force = calculateForce(body3, body1);

        body3.acceleration.x = force.x / body3.mass;
        body3.acceleration.y = force.y / body3.mass;

        body3.velocity.x += body3.acceleration.x;
        body3.velocity.y += body3.acceleration.y;

        body3.position.x += body3.velocity.x;
        body3.position.y += body3.velocity.y;

        force = calculateForce(body3, body2);

        body3.acceleration.x = force.x / body3.mass;
        body3.acceleration.y = force.y / body3.mass;

        body3.velocity.x += body3.acceleration.x;
        body3.velocity.y += body3.acceleration.y;

        body3.position.x += body3.velocity.x;
        body3.position.y += body3.velocity.y;

        BeginDrawing();

        ClearBackground(WHITE);

        DrawCircle((body1.position.x + mouseOffsetX) * zoom + winWidth / 2, (body1.position.y + mouseOffsetY) * zoom + winHeight / 2, 25 * zoom, BLACK);
        DrawCircle((body2.position.x + mouseOffsetX) * zoom + winWidth / 2, (body2.position.y + mouseOffsetY) * zoom + winHeight / 2, 25 * zoom, BLACK);
        DrawCircle((body3.position.x + mouseOffsetX) * zoom + winWidth / 2, (body3.position.y + mouseOffsetY) * zoom + winHeight / 2, 25 * zoom, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
