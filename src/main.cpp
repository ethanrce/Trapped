#include <raylib.h>
#include <raymath.h>
#include <iostream>
using std::cout;
#include "screens.h"
#include "objects.h"
#define SCREENHEIGHT 850
#define SCREENWIDTH 600

GameScreen CurrentScreen;
int display;

int main(void) {
    InitWindow(SCREENHEIGHT, SCREENWIDTH, "Trapped");
    InitMain();
   // SetWindowState(FLAG_WINDOW_RESIZABLE);
   // SetWindowMinSize(300, 300);
    switch(CurrentScreen) {
        case LOGO: break;
        case ANIM: break;
        case GAME: InitGame(); break;
    }

    while (!WindowShouldClose()) {
        UpdateDrawingFrame();
    }

    UnloadGame();
    CloseWindow();
    return 0;
}

void InitMain(void) {
    CurrentScreen = GAME;
    display = GetCurrentMonitor();
   // SetTargetFPS(GetMonitorRefreshRate(display));
   SetTargetFPS(60);
}

void UpdateDrawingFrame(void) {
    switch(CurrentScreen) {
            case LOGO: break;
            case ANIM: break;
            case GAME: UpdateGame(); break;
    }

    BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(CurrentScreen) {
            case LOGO: break;
            case ANIM: break;
            case GAME: DrawGame(); break;
    }
    EndDrawing();
}

void UnloadMain(void) {
}

Object makeObject(Texture2D png, float x, float y, float rot, Vector2 origin, Rectangle draw) {
    Object obj;
    obj.texture = png;
    obj.draw = draw;
    obj.position = Rectangle{x, y, (float) obj.draw.width, obj.draw.height};
    obj.origin = origin;
    obj.rotation = rot;
    return obj;
} 