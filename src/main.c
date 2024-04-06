#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define G 6.674e-11
#define MAX_PARTICLES 300
#define MAX_BODIES 4

typedef struct {
    int radius;
    float mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
} Body;

typedef struct {
    Vector2 position;
} Particle;

int main() {
    const int winWidth = 800;
    const int winHeight = 450;

    Body bodies[MAX_BODIES] = {
        {17, 500000000000, {winWidth / 2, -winHeight * 2}, {5, 0}, {0, 0}},
        {25, 5000000000000, {winWidth * 3, winHeight / 2}, {0, 4}, {0, 0}},
        {25, 5000000000000, {winWidth / 2, winHeight * 3}, {-4, 0}, {0, 0}},
        {200, 1000000000000000, {winWidth / 2, winHeight / 2}, {0, 0}, {0, 0}},
    };

    Particle particles[MAX_PARTICLES];
    int particleIndex = 0;
    int bodyIndex = 0;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].position.x = bodies[bodyIndex].position.x;
        particles[i].position.y = bodies[bodyIndex].position.y;
        bodyIndex = (bodyIndex + 1) % MAX_BODIES;
    }

    int lastMousePosX = 0;
    int lastMousePosY = 0;
    float mouseOffsetX = -winWidth / 2;
    float mouseOffsetY = -winHeight / 2;

    float zoom = 1.0f;
    float zoomSpeed = 0.05f;

    double distance = 0;
    double force = 0;
    double angle = 0;
    double forceX = 0;
    double forceY = 0;

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

        for (int i = 0; i < MAX_BODIES; i++) {
            for (int j = 0; j < MAX_BODIES; j++) {
                if (i != j) {
                    distance = sqrt(pow((bodies[j].position.x - bodies[i].position.x), 2) + pow((bodies[j].position.y - bodies[i].position.y), 2));
                    force = G * bodies[i].mass * bodies[j].mass / pow(distance, 2);
                    angle = atan2(bodies[j].position.y - bodies[i].position.y, bodies[j].position.x - bodies[i].position.x);
                    forceX = force * cos(angle);
                    forceY = force * sin(angle);

                    bodies[i].acceleration.x = forceX / bodies[i].mass;
                    bodies[i].acceleration.y = forceY / bodies[i].mass;

                    bodies[i].velocity.x += bodies[i].acceleration.x;
                    bodies[i].velocity.y += bodies[i].acceleration.y;

                    bodies[i].position.x += bodies[i].velocity.x;
                    bodies[i].position.y += bodies[i].velocity.y;
                }
            }
        }

        particles[particleIndex].position.x = bodies[bodyIndex].position.x;
        particles[particleIndex].position.y = bodies[bodyIndex].position.y;
        particleIndex = (particleIndex + 1) % MAX_PARTICLES;
        bodyIndex = (bodyIndex + 1) % MAX_BODIES;

        BeginDrawing();

        ClearBackground(WHITE);

        for (int i = 0; i < MAX_PARTICLES; i++) {
            DrawCircle((particles[i].position.x + mouseOffsetX) * zoom + winWidth / 2, (particles[i].position.y + mouseOffsetY) * zoom + winHeight / 2, 5 * zoom, LIGHTGRAY);
        }

        for (int i = 0; i < MAX_BODIES; i++) {
            DrawCircle((bodies[i].position.x + mouseOffsetX) * zoom + winWidth / 2, (bodies[i].position.y + mouseOffsetY) * zoom + winHeight / 2, bodies[i].radius * zoom, BLACK);
        }

        DrawText(TextFormat("%d", GetFPS()), 5, 5, 25, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
