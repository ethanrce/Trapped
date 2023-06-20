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

Object makeObject(Texture2D png) {
    Object obj;
    obj.texture = png;
    obj.draw = Rectangle{0, 0, (float) png.width/2.0f, (float) png.height};;
    obj.position = Rectangle{(float) GetScreenWidth()/2.0f, (float) GetScreenHeight()/2.0f, (float) obj.draw.width, obj.draw.height};
    obj.origin = Vector2{obj.position.width/2.0f, obj.position.height/2.0f};
    obj.rotation = 0.0f;
    return obj;
} 