#include "raylib.h"
#include "raymath.h"
#include "screens.h"
#include <iostream>
using namespace std;
#include "objects.h"

Texture2D PlayButton;
Object Play;
Vector2 mousePoint; // Gets mouse's position for button collision
bool btnAction;
int btnState;
bool EndMainMenu; 
Sound buttonsound;

void InitMainMenu(void) {
    PlayButton = LoadTexture("assets/PlayButton.png");
    Play = makeObject(PlayButton, (GetScreenWidth()/2.0f) - (PlayButton.width/2.0f), (GetScreenHeight()/2.0f) - (PlayButton.height/6.0f), 0.0f, (Vector2){0.0f, 0.0f}, (Rectangle){0.0f, 0.0f, (float) PlayButton.width, (float) PlayButton.height/3.0f}, "neither");
    buttonsound = LoadSound("assets/Audio/PlayButtonfx.wav");
    mousePoint = {0.0f, 0.0f};
    EndMainMenu = false;
    btnAction = false;
    btnState = 0;
}

void UpdateMainMenu(void) {
    mousePoint = GetMousePosition();
    btnAction = false;
     if (CheckCollisionPointRec(mousePoint, Play.position)) {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            btnState = 2;
        } else {
            btnState = 1;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            cout << "Clicked" << endl;
            EndMainMenu = true;
            PlaySound(buttonsound);
        }
    } else {
        btnState = 0;
    }
    Play.draw.y = PlayButton.height/3.0f * btnState;
}

void DrawMainMenu(void) {
    DrawTexturePro(PlayButton, Play.draw, Play.position, Play.origin, Play.rotation, RAYWHITE);
    DrawRectangleLines(Play.position.x, Play.position.y, Play.position.width, Play.position.height, RED);
}

void UnloadMainMenu(void) {
    UnloadTexture(PlayButton);
    UnloadSound(buttonsound);
}

bool FinishMainMenu(void) {
    return EndMainMenu;
}
