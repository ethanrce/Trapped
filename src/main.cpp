#include <raylib.h>
#include <raymath.h>
#include <iostream>
using std::cout;
#include "screens.h"
#include "objects.h"
#define SCREENWIDTH 850
#define SCREENHEIGHT 600

GameScreen CurrentScreen;
int display;

int main(void) {
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Trapped");
    InitMain();
   // SetWindowState(FLAG_WINDOW_RESIZABLE);
   // SetWindowMinSize(300, 300);
    switch(CurrentScreen) {
        case LOGO: InitLogoScreen(); break;
        case MAINMENU: InitMainMenu(); break;
        case GAME: InitGame(); break;
    }

    while (!WindowShouldClose()) {
        UpdateDrawingFrame();
    }

    switch(CurrentScreen) {
        case LOGO: UnloadLogoScreen(); break;
        case MAINMENU: UnloadMainMenu(); break;
        case GAME: UnloadGame(); break;
    }
    UnloadMain();

    CloseWindow();
    return 0;
}

void InitMain(void) {
    srand(time(NULL));
    CurrentScreen = MAINMENU;
    display = GetCurrentMonitor();
    // SetTargetFPS(GetMonitorRefreshRate(display));
    SetTargetFPS(60);
}

void UpdateDrawingFrame(void) {
    switch(CurrentScreen) {
            case LOGO: {
            UpdateLogoScreen();
            if (FinishLogoScreen()) {
                UnloadLogoScreen();
                //PlayMusicStream(gamesound);
                InitMainMenu();
                CurrentScreen = MAINMENU;
            }
            } break;
            case MAINMENU: {
            UpdateMainMenu();
            if (FinishMainMenu()) {
                UnloadMainMenu();
                InitGame();
                CurrentScreen = GAME;
            }
            } break;
            case GAME: {
                UpdateGame();
                if (FinishGame()) {
                    UnloadGame();
                    InitMainMenu();
                    CurrentScreen = MAINMENU;
                }
            } break;
    }

    BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(CurrentScreen) {
            case LOGO: DrawLogoScreen(); break;
            case MAINMENU: DrawMainMenu(); break;
            case GAME: DrawGame(); break;
    }
    EndDrawing();
}

void UnloadMain(void) {
}

float RandomNum(int min, int max) {
    return min + (rand() % max);
}

Object makeObject(Texture2D png, float x, float y, float rot, Vector2 origin, Rectangle draw, const char *sidechoice) {
    Object obj;
    obj.texture = png;
    obj.draw = draw;
    obj.position = Rectangle{x, y, (float) obj.draw.width, obj.draw.height};
    obj.origin = origin;
    obj.rotation = rot;
    obj.direction = "forward";
    obj.side = sidechoice;
    if (std::string(obj.side) == "left") {
        obj.tongue = (Rectangle){obj.position.x + obj.position.width/2.0f, obj.position.y, 11, 3};
    } else {
        obj.tongue = (Rectangle){obj.position.x - obj.position.width/2.0f - 2, obj.position.y, 11, 3};
    }
    return obj;
} 