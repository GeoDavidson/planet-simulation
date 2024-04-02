#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#define G 6.674e-11
#define MAX_PARTICLES 1000

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

    Body body1 = {{winWidth / 3, winHeight / 2}, {0, 1}, {0, 0}, 500000000000};
    Body body2 = {{winWidth / 2, winHeight / 2}, {0, 0}, {0, 0}, 5000000000000};
    Body body3 = {{winWidth / 1.5, winHeight / 2}, {0, -1}, {0, 0}, 5000000000000};

    Particle particles1[MAX_PARTICLES];
    int particleIndex1 = 0;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles1[i].position.x = body1.position.x;
        particles1[i].position.y = body1.position.y;
    }

    Particle particles2[MAX_PARTICLES];
    int particleIndex2 = 0;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles2[i].position.x = body2.position.x;
        particles2[i].position.y = body2.position.y;
    }

    Particle particles3[MAX_PARTICLES];
    int particleIndex3 = 0;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles3[i].position.x = body3.position.x;
        particles3[i].position.y = body3.position.y;
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

        particles1[particleIndex1].position.x = body1.position.x;
        particles1[particleIndex1].position.y = body1.position.y;

        particleIndex1 = (particleIndex1 + 1) % MAX_PARTICLES;

        particles2[particleIndex2].position.x = body2.position.x;
        particles2[particleIndex2].position.y = body2.position.y;

        particleIndex2 = (particleIndex2 + 1) % MAX_PARTICLES;

        particles3[particleIndex3].position.x = body3.position.x;
        particles3[particleIndex3].position.y = body3.position.y;

        particleIndex3 = (particleIndex3 + 1) % MAX_PARTICLES;

        BeginDrawing();

        ClearBackground(WHITE);

        for (int i = 0; i < MAX_PARTICLES; i++) {
            DrawCircle((particles1[i].position.x + mouseOffsetX) * zoom + winWidth / 2, (particles1[i].position.y + mouseOffsetY) * zoom + winHeight / 2, 5 * zoom, LIGHTGRAY);
            DrawCircle((particles2[i].position.x + mouseOffsetX) * zoom + winWidth / 2, (particles2[i].position.y + mouseOffsetY) * zoom + winHeight / 2, 5 * zoom, LIGHTGRAY);
            DrawCircle((particles3[i].position.x + mouseOffsetX) * zoom + winWidth / 2, (particles3[i].position.y + mouseOffsetY) * zoom + winHeight / 2, 5 * zoom, LIGHTGRAY);
        }

        DrawCircle((body1.position.x + mouseOffsetX) * zoom + winWidth / 2, (body1.position.y + mouseOffsetY) * zoom + winHeight / 2, 25 * zoom, BLACK);
        DrawCircle((body2.position.x + mouseOffsetX) * zoom + winWidth / 2, (body2.position.y + mouseOffsetY) * zoom + winHeight / 2, 25 * zoom, BLACK);
        DrawCircle((body3.position.x + mouseOffsetX) * zoom + winWidth / 2, (body3.position.y + mouseOffsetY) * zoom + winHeight / 2, 25 * zoom, BLACK);

        DrawText(TextFormat("%d", GetFPS()), 5, 5, 25, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
