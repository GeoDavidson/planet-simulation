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
    struct body *next;
} body_t;

void newBody(body_t **head, body_t body) {
    body_t *newBody = (body_t *)malloc(sizeof(body_t));
    newBody->radius = body.radius;
    newBody->mass = body.mass;
    newBody->position = body.position;
    newBody->velocity = body.velocity;
    newBody->acceleration = body.acceleration;
    newBody->next = NULL;

    if (*head == NULL) {
        *head = newBody;
    } else {
        body_t *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newBody;
    }
}

void pop(body_t **head) {
    if (*head == NULL) {
        fprintf(stderr, "ERROR: underflow, can't pop from empty list\n");
        exit(1);
    }
    body_t *nextBody = (*head)->next;
    free(*head);
    *head = nextBody;
}

void popLast(body_t **head) {
    if (*head == NULL) {
        fprintf(stderr, "ERROR: underflow, can't pop from empty list\n");
        exit(1);
    } else if ((*head)->next == NULL) {
        body_t *nextBody = (*head)->next;
        free(*head);
        *head = nextBody;
    } else {
        body_t *current = *head;
        while (current->next->next != NULL) {
            current = current->next;
        }
        free(current->next);
        current->next = NULL;
    }
}

void popIndex(body_t **head, int index) {
    if (index == 0) {
        pop(head);
    } else {
        body_t *current = *head;
        for (int i = 0; i < index - 1; i++) {
            if (current->next == NULL) {
                fprintf(stderr, "ERROR: index out of range\n");
                exit(1);
            }
            current = current->next;
        }
        if (current->next == NULL) {
            fprintf(stderr, "ERROR: index out of range\n");
            exit(1);
        }
        body_t *tempNode = current->next;
        current->next = tempNode->next;
        free(tempNode);
    }
}

int main() {
    const int winWidth = 800;
    const int winHeight = 450;

    InitWindow(winWidth, winHeight, "basic window");

    body_t *head = NULL;

    body_t bodies[4] = {
        {15, pow(3, 2) * 640000000000, {winWidth / 2, -winHeight * 2}, {5, 0}, {0, 0}, NULL},
        {25, pow(5, 2) * 640000000000, {winWidth * 3, winHeight / 2}, {0, 4}, {0, 0}, NULL},
        {25, pow(5, 2) * 640000000000, {winWidth / 2, winHeight * 3}, {-4, 0}, {0, 0}, NULL},
        {200, pow(40, 2) * 640000000000, {winWidth / 2, winHeight / 2}, {0, 0}, {0, 0}, NULL},
    };

    newBody(&head, bodies[0]);
    newBody(&head, bodies[1]);
    newBody(&head, bodies[3]);
    newBody(&head, bodies[2]);

    body_t *currentIndex1 = head;
    body_t *currentIndex2 = head;

    Camera2D camera = {0};
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

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            mouseDelta = GetMouseDelta();
            camera.target.x += -1 * mouseDelta.x / camera.zoom;
            camera.target.y += -1 * mouseDelta.y / camera.zoom;
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            pop(&head);
        }

        if (head != NULL) {
            if (head->next == NULL) {
                head->position.x += head->velocity.x;
                head->position.y += head->velocity.y;
            }
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
                currentIndex2 = currentIndex2->next;
            }
            currentIndex1 = currentIndex1->next;
        }

        BeginDrawing();

        ClearBackground(WHITE);

        BeginMode2D(camera);

        currentIndex1 = head;
        while (currentIndex1 != NULL) {
            DrawCircle((currentIndex1->position.x), (currentIndex1->position.y), currentIndex1->radius, BLACK);
            currentIndex1 = currentIndex1->next;
        }

        EndMode2D();

        DrawText(TextFormat("%d", GetFPS()), 5, 5, 25, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
