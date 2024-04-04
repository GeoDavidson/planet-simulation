#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define G 6.674e-11
#define MAX_PARTICLES 1000
#define MAX_BODIES 3

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
} Body;

typedef struct {
    Vector2 position;
} Particle;

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

    Body bodies[MAX_BODIES] = {
        {{winWidth / 3, winHeight / 2}, {0, 1}, {0, 0}, 500000000000},
        {{winWidth / 2, winHeight / 2}, {0, 0}, {0, 0}, 5000000000000},
        {{winWidth / 1.5, winHeight / 2}, {0, -1}, {0, 0}, 5000000000000}
    };

    Particle particles1[MAX_PARTICLES];
    int particleIndex1 = 0;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles1[i].position.x = bodies[0].position.x;
        particles1[i].position.y = bodies[0].position.y;
    }

    Particle particles2[MAX_PARTICLES];
    int particleIndex2 = 0;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles2[i].position.x = bodies[1].position.x;
        particles2[i].position.y = bodies[1].position.y;
    }

    Particle particles3[MAX_PARTICLES];
    int particleIndex3 = 0;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles3[i].position.x = bodies[2].position.x;
        particles3[i].position.y = bodies[2].position.y;
    }

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

        for (int i = 0; i < MAX_BODIES; i++) {
            for (int j = 0; j < MAX_BODIES; j++) {
                if (i != j) {
                    force = calculateForce(bodies[i], bodies[j]);

                    bodies[i].acceleration.x = force.x / bodies[i].mass;
                    bodies[i].acceleration.y = force.y / bodies[i].mass;

                    bodies[i].velocity.x += bodies[i].acceleration.x;
                    bodies[i].velocity.y += bodies[i].acceleration.y;

                    bodies[i].position.x += bodies[i].velocity.x;
                    bodies[i].position.y += bodies[i].velocity.y;
                }
            }
        }

        particles1[particleIndex1].position.x = bodies[0].position.x;
        particles1[particleIndex1].position.y = bodies[0].position.y;

        particleIndex1 = (particleIndex1 + 1) % MAX_PARTICLES;

        particles2[particleIndex2].position.x = bodies[1].position.x;
        particles2[particleIndex2].position.y = bodies[1].position.y;

        particleIndex2 = (particleIndex2 + 1) % MAX_PARTICLES;

        particles3[particleIndex3].position.x = bodies[2].position.x;
        particles3[particleIndex3].position.y = bodies[2].position.y;

        particleIndex3 = (particleIndex3 + 1) % MAX_PARTICLES;

        BeginDrawing();

        ClearBackground(WHITE);

        for (int i = 0; i < MAX_PARTICLES; i++) {
            DrawCircle((particles1[i].position.x + mouseOffsetX) * zoom + winWidth / 2, (particles1[i].position.y + mouseOffsetY) * zoom + winHeight / 2, 5 * zoom, LIGHTGRAY);
            DrawCircle((particles2[i].position.x + mouseOffsetX) * zoom + winWidth / 2, (particles2[i].position.y + mouseOffsetY) * zoom + winHeight / 2, 5 * zoom, LIGHTGRAY);
            DrawCircle((particles3[i].position.x + mouseOffsetX) * zoom + winWidth / 2, (particles3[i].position.y + mouseOffsetY) * zoom + winHeight / 2, 5 * zoom, LIGHTGRAY);
        }

        DrawCircle((bodies[0].position.x + mouseOffsetX) * zoom + winWidth / 2, (bodies[0].position.y + mouseOffsetY) * zoom + winHeight / 2, 25 * zoom, BLACK);
        DrawCircle((bodies[1].position.x + mouseOffsetX) * zoom + winWidth / 2, (bodies[1].position.y + mouseOffsetY) * zoom + winHeight / 2, 25 * zoom, BLACK);
        DrawCircle((bodies[2].position.x + mouseOffsetX) * zoom + winWidth / 2, (bodies[2].position.y + mouseOffsetY) * zoom + winHeight / 2, 25 * zoom, BLACK);

        DrawText(TextFormat("%d", GetFPS()), 5, 5, 25, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
