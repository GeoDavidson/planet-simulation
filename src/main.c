#include <math.h>
#include <raylib.h>
#include <stdio.h>

double G = 6.67430e+1; // 6.674e-11

typedef struct {
    Vector2 position;
    Vector2 velocity;
    Vector2 acceleration;
    float mass;
} Body;

int main() {
    const int winWidth = 800;
    const int winHeight = 450;

    Body body1 = {{winWidth / 3, winHeight / 2}, {0, 4}, {0, 0}, 42};
    Body body2 = {{winWidth / 2, winHeight / 2}, {0, -4}, {0, 0}, 42};

    float distance = sqrt(pow((body2.position.x - body1.position.x), 2) + pow((body2.position.y - body1.position.y), 2));
    float force = G * body1.mass * body2.mass / pow(distance, 2);
    float angle = atan2(body2.position.y - body1.position.y, body2.position.x - body1.position.x);
    float forceX = force * cos(angle);
    float forceY = force * sin(angle);

    body1.acceleration.x = forceX / body1.mass;
    body1.acceleration.y = forceY / body1.mass;

    body1.velocity.x = body1.velocity.x + body1.acceleration.x;
    body1.velocity.y = body1.velocity.y + body1.acceleration.y;

    body1.position.x = body1.position.x + body1.velocity.x;
    body1.position.y = body1.position.y + body1.velocity.y;

    InitWindow(winWidth, winHeight, "basic window");

    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        distance = sqrt(pow((body2.position.x - body1.position.x), 2) + pow((body2.position.y - body1.position.y), 2));
        force = G * body1.mass * body2.mass / pow(distance, 2);
        angle = atan2(body2.position.y - body1.position.y, body2.position.x - body1.position.x);
        forceX = force * cos(angle);
        forceY = force * sin(angle);

        body1.acceleration.x = forceX / body1.mass;
        body1.acceleration.y = forceY / body1.mass;

        body1.velocity.x = body1.velocity.x + body1.acceleration.x;
        body1.velocity.y = body1.velocity.y + body1.acceleration.y;

        body1.position.x = body1.position.x + body1.velocity.x;
        body1.position.y = body1.position.y + body1.velocity.y;

        distance = sqrt(pow((body1.position.x - body2.position.x), 2) + pow((body1.position.y - body2.position.y), 2));
        force = G * body2.mass * body1.mass / pow(distance, 2);
        angle = atan2(body1.position.y - body2.position.y, body1.position.x - body2.position.x);
        forceX = force * cos(angle);
        forceY = force * sin(angle);

        body2.acceleration.x = forceX / body2.mass;
        body2.acceleration.y = forceY / body2.mass;

        body2.velocity.x = body2.velocity.x + body2.acceleration.x;
        body2.velocity.y = body2.velocity.y + body2.acceleration.y;

        body2.position.x = body2.position.x + body2.velocity.x;
        body2.position.y = body2.position.y + body2.velocity.y;

        BeginDrawing();

        ClearBackground(WHITE);

        DrawCircle(body1.position.x, body1.position.y, 25, BLACK);
        DrawCircle(body2.position.x, body2.position.y, 50, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
