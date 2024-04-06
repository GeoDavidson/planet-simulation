#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define G 6.674e-11
#define ZOOM_SPEED 0.025f

typedef struct body {
    int radius;
    float mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    struct body *nextBody;
} body_t;

void newBody(body_t **head, int radius, float mass, Vector2 position, Vector2 velocity, Vector2 acceleration) {
    body_t *newBody = (body_t *)malloc(sizeof(body_t));
    newBody->radius = radius;
    newBody->mass = mass;
    newBody->position = position;
    newBody->velocity = velocity;
    newBody->acceleration = acceleration;
    newBody->nextBody = NULL;

    if (*head == NULL) {
        *head = newBody;
    } else {
        body_t *current = *head;

        while (current->nextBody != NULL) {
            current = current->nextBody;
        }

        current->nextBody = newBody;
    }
}

int main() {
    const int winWidth = 800;
    const int winHeight = 450;

    InitWindow(winWidth, winHeight, "basic window");

    body_t *head = NULL;

    body_t bodies[4] = {
        {17, 500000000000, {winWidth / 2, -winHeight * 2}, {5, 0}, {0, 0}},
        {25, 5000000000000, {winWidth * 3, winHeight / 2}, {0, 4}, {0, 0}},
        {25, 5000000000000, {winWidth / 2, winHeight * 3}, {-4, 0}, {0, 0}},
        {200, 1000000000000000, {winWidth / 2, winHeight / 2}, {0, 0}, {0, 0}},
    };

    newBody(&head, bodies[0].radius, bodies[0].mass, bodies[0].position, bodies[0].velocity, bodies[0].acceleration);
    newBody(&head, bodies[1].radius, bodies[1].mass, bodies[1].position, bodies[1].velocity, bodies[1].acceleration);
    newBody(&head, bodies[2].radius, bodies[2].mass, bodies[2].position, bodies[2].velocity, bodies[2].acceleration);
    newBody(&head, bodies[3].radius, bodies[3].mass, bodies[3].position, bodies[3].velocity, bodies[3].acceleration);

    body_t *currentIndex1 = head;
    body_t *currentIndex2 = head;

    Camera2D camera;
    camera.offset.x = winWidth / 2;
    camera.offset.y = winHeight / 2;
    camera.target.x = winWidth / 2;
    camera.target.y = winHeight / 2;
    camera.zoom = 0.25f;

    Vector2 mouseDelta = {0.0f, 0.0f};
    float mouseWheel = 0.0f;

    double distance = 0;
    double force = 0;
    double angle = 0;
    double forceX = 0;
    double forceY = 0;

    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        mouseWheel = GetMouseWheelMove();
        if (mouseWheel != 0) {
            camera.target = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.zoom = Clamp(camera.zoom + mouseWheel * ZOOM_SPEED, ZOOM_SPEED, 10.0f);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            mouseDelta = GetMouseDelta();
            camera.target.x += -1 * mouseDelta.x / camera.zoom;
            camera.target.y += -1 * mouseDelta.y / camera.zoom;
        }

        currentIndex1 = head;
        while (currentIndex1 != NULL) {
            currentIndex2 = head;
            while (currentIndex2 != NULL) {
                if (currentIndex1 != currentIndex2) {
                    distance = sqrt(pow((currentIndex2->position.x - currentIndex1->position.x), 2) + pow((currentIndex2->position.y - currentIndex1->position.y), 2));
                    force = G * currentIndex1->mass * currentIndex2->mass / pow(distance, 2);
                    angle = atan2(currentIndex2->position.y - currentIndex1->position.y, currentIndex2->position.x - currentIndex1->position.x);
                    forceX = force * cos(angle);
                    forceY = force * sin(angle);

                    currentIndex1->acceleration.x = forceX / currentIndex1->mass;
                    currentIndex1->acceleration.y = forceY / currentIndex1->mass;

                    currentIndex1->velocity.x += currentIndex1->acceleration.x;
                    currentIndex1->velocity.y += currentIndex1->acceleration.y;

                    currentIndex1->position.x += currentIndex1->velocity.x;
                    currentIndex1->position.y += currentIndex1->velocity.y;
                }
                currentIndex2 = currentIndex2->nextBody;
            }
            currentIndex1 = currentIndex1->nextBody;
        }

        BeginDrawing();

        ClearBackground(WHITE);

        BeginMode2D(camera);

        currentIndex1 = head;
        while (currentIndex1 != NULL) {
            DrawCircle((currentIndex1->position.x), (currentIndex1->position.y), currentIndex1->radius, BLACK);
            currentIndex1 = currentIndex1->nextBody;
        }

        EndMode2D();

        DrawText(TextFormat("%d", GetFPS()), 5, 5, 25, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
