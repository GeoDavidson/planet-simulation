#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define G 6.674e-11
#define MAX_PARTICLES 2000
#define INIT_BODIES 4

typedef struct particle {
    int radius;
    Color color;
    Vector2 position;
} particle_t;

typedef struct body {
    int radius;
    float mass;
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    struct body *nextBody;
} body_t;

void printList(body_t *head) {
    body_t *current = head;
    while (current != NULL) {
        printf("%d, %f, %f, %f\n", current->radius, current->mass, current->position.x, current->position.y);
        current = current->nextBody;
    }
}

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

    body_t *head = NULL;

    body_t bodies[INIT_BODIES] = {
        {17, 500000000000, {winWidth / 2, -winHeight * 2}, {5, 0}, {0, 0}},
        {25, 5000000000000, {winWidth * 3, winHeight / 2}, {0, 4}, {0, 0}},
        {25, 5000000000000, {winWidth / 2, winHeight * 3}, {-4, 0}, {0, 0}},
        {200, 1000000000000000, {winWidth / 2, winHeight / 2}, {0, 0}, {0, 0}},
    };

    newBody(&head, 17, 500000000000, bodies[0].position, bodies[0].velocity, bodies[0].acceleration);
    newBody(&head, 25, 5000000000000, bodies[1].position, bodies[1].velocity, bodies[1].acceleration);
    newBody(&head, 25, 5000000000000, bodies[2].position, bodies[2].velocity, bodies[2].acceleration);
    newBody(&head, 200, 1000000000000000, bodies[3].position, bodies[3].velocity, bodies[3].acceleration);

    body_t *currentIndex1 = head;
    body_t *currentIndex2 = head;

    particle_t particles[MAX_PARTICLES];
    int particleIndex = 0;

    Color colors[3] = {RED, GREEN, BLUE};
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particles[i].radius = i % 3 * 2 + 5;
        particles[i].color = colors[i % 3];
        particles[i].position.x = 0;
        particles[i].position.y = 0;
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

        currentIndex1 = head;
        while (currentIndex1 != NULL) {
            particles[particleIndex].position.x = currentIndex1->position.x;
            particles[particleIndex].position.y = currentIndex1->position.y;
            particleIndex = (particleIndex + 1) % MAX_PARTICLES;
            currentIndex1 = currentIndex1->nextBody;
        }

        BeginDrawing();

        ClearBackground(WHITE);

        for (int i = 0; i < MAX_PARTICLES; i++) {
            DrawCircle((particles[i].position.x + mouseOffsetX) * zoom + winWidth / 2, (particles[i].position.y + mouseOffsetY) * zoom + winHeight / 2, particles[i].radius * zoom, particles[i].color);
        }

        currentIndex1 = head;
        while (currentIndex1 != NULL) {
            DrawCircle((currentIndex1->position.x + mouseOffsetX) * zoom + winWidth / 2, (currentIndex1->position.y + mouseOffsetY) * zoom + winHeight / 2, currentIndex1->radius * zoom, BLACK);
            currentIndex1 = currentIndex1->nextBody;
        }

        DrawText(TextFormat("%d", GetFPS()), 5, 5, 25, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
