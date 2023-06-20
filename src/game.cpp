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
NPatchInfo gamebackgroundinfo;
Rectangle LeftBorder;
Rectangle RightBorder;
Texture2D flyswatterpng;
Object tempflyswatter;
Texture2D frogpng;
Object tempfrog;
Texture2D spiderpng;
Object tempspider;

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
    gamebackgroundinfo = {(Rectangle) {0.0f, 0.0f, (float) gamebackground.width, (float) gamebackground.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    RightBorder = (Rectangle){611, 46, 4, 500};
    LeftBorder = (Rectangle){253, 46, 4, 500};
}

void UpdateGame(void) {
    Animations();
    Movement();
   // camera.zoom = GetScreenWidth()/800.0f;
    camera.target = (Vector2){fly.position.x, fly.position.y};
}

void DrawGame(void) {
   
    BeginMode2D(camera);
        DrawTextureNPatch(gamebackground, gamebackgroundinfo, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE);
        DrawRectangleRec(LeftBorder, SKYBLUE);
        DrawRectangleRec(RightBorder, SKYBLUE);
        //DrawTexture(gamebackground, GetScreenWidth()/2.0f, GetScreenHeight()/2.0f, RAYWHITE);
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

    if (fly.position.x - fly.position.width/2.0f <= LeftBorder.x + LeftBorder.width) {
        fly.position.x = LeftBorder.x + LeftBorder.width + (fly.position.width/2.0f) + 1;
    } else if (fly.position.x + fly.position.width/2.0f >= RightBorder.x) {
        fly.position.x = RightBorder.x - (fly.position.width/2.0f) - 1;
    }
}  

bool CheckCollisions(void) {
    return false;
}

void UnloadGame(void) {
    UnloadTexture(flypng);
    UnloadTexture(gamebackground);
}