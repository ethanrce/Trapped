#include <raylib.h>
#include <raymath.h>
#include "screens.h"
#include "objects.h"

#define FPS 60
#define FRAMESPEED 8 // Seconds between each animation frame
#define FLYROTATION 3.0f
#define FLYSPEED 5.0f
Object fly;
Texture2D flypng;
int flycounter;
int flyframe;
Camera2D camera;
Texture2D gamebackground;

void InitGame(void) {
    flypng = LoadTexture("assets/Fly.png");
    gamebackground = LoadTexture("assets/background.png");
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
    Animations();
    Movement();
 //   camera.target = (Vector2){fly.position.x, fly.position.y};
}

void DrawGame(void) {
    BeginMode2D(camera);
        DrawTexture(gamebackground, 0, 0, RAYWHITE);
        DrawTexturePro(flypng, fly.draw, fly.position, fly.origin, fly.rotation, RAYWHITE);
        DrawCircleLines(fly.position.x, fly.position.y, fly.position.width/2.0f, BLUE);
    EndMode2D();
    DrawFPS(10, 10);
}

void Animations(void) {
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

void Movement(void) {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
     //   if (fly.position.x < (CheckWidth() - (fly.position.width/2.0f))) {
            fly.rotation += FLYROTATION;
       // }
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
       // if ((int)fly.position.x >= (fly.position.width/2.0f)) {
            fly.rotation -= FLYROTATION;
       // }
    }
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        fly.position.x += FLYSPEED * sinf(fly.rotation * (PI / 180.0f));
        fly.position.y -= FLYSPEED * cosf(fly.rotation * (PI / 180.0f));        
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)){
        fly.position.x -= FLYSPEED * sinf(fly.rotation * (PI / 180.0f));
        fly.position.y += FLYSPEED * cosf(fly.rotation * (PI / 180.0f));
    } 
}  

void UnloadGame(void) {
    UnloadTexture(flypng);
    UnloadTexture(gamebackground);
}