#include <raylib.h>
#include <raymath.h>
#include <iostream>
using std::cout;

#define SCREENHEIGHT 800
#define SCREENWIDTH 800

void UpdateDrawingFrame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    EndDrawing();
}

int main(void) {
    InitWindow(SCREENHEIGHT, SCREENWIDTH, "GameJam");

    while (!WindowShouldClose()) {
        UpdateDrawingFrame();
    }

    CloseWindow();

    return 0;
}

