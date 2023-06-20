#include <raylib.h>
#include "screens.h"
#include "objects.h"

#define FPS 60
#define FRAMESPEED 8 // Seconds between each animation frame
Object fly;
Texture2D flypng;
int flycounter;
int flyframe;
Camera2D camera;

void InitGame(void) {
    flypng = LoadTexture("assets/Fly.png");
    fly = makeObject(flypng);
    flycounter = 0;
    flyframe = 0;
    camera = {0};
    camera.target = (Vector2){fly.position.x, fly.position.y};
    camera.offset = (Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void UpdateGame(void) {
    camera.target = (Vector2){fly.position.x, fly.position.y};
    flycounter ++;
    if (flycounter >= (float)(FPS/FRAMESPEED)) {
        flycounter = 0;
        flyframe ++;
        if (flyframe >= 3) {
            flyframe = 1;
        }
        fly.draw.x = (fly.texture.width/2.0f) * flyframe;
    }
}

void DrawGame(void) {
    BeginMode2D(camera);
        DrawTexturePro(flypng, fly.draw, fly.position, fly.origin, fly.rotation, RAYWHITE);
    EndMode2D();

    DrawFPS(10, 10);
}

void UnloadGame(void) {
    UnloadTexture(flypng);
}