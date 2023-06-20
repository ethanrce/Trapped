#include <raylib.h>
#include <raymath.h>
#include "screens.h"
#include "objects.h"
#include <vector>
#include <iostream>
using namespace std;


#define FPS 60
#define FRAMESPEED 8 // Seconds between each animation frame
#define FLYROTATION 3.0f
#define FLYSPEED 5.0f
#define FLYSWATTERROTATION 5.0f
#define TONGUESPEED 0.5f
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
Texture2D spiderwebpng;
vector<Object> spiders;
Object tempspider;
Object tempspiderweb;
vector<Object> flyswatters;
vector<Object> frogs;

void InitGame(void) {
    RightBorder = (Rectangle){611, 46, 4, 500};
    LeftBorder = (Rectangle){253, 46, 4, 500};
    flypng = LoadTexture("assets/Fly.png");
    fly = makeObject(flypng, GetScreenWidth()/2.0f, GetScreenWidth()/2.0f, 0.0f, (Vector2){flypng.width/4.0f, flypng.height/2.0f}, (Rectangle){0, 0, (float) flypng.width/2.0f, (float) flypng.height});
    gamebackground = LoadTexture("assets/background.png");
    flyswatterpng = LoadTexture("assets/FlySwatter.png");
    tempflyswatter = makeObject(flyswatterpng, LeftBorder.x + LeftBorder.width, 250.0f, 90.0f, (Vector2){ (float) flyswatterpng.width/2.0f, (float) flyswatterpng.height}, (Rectangle) {0.0f, 0.0f, (float) flyswatterpng.width, (float) flyswatterpng.height});
    flyswatters.push_back(tempflyswatter);

    frogpng = LoadTexture("assets/Frog.png");
    tempfrog = makeObject(frogpng, LeftBorder.x + LeftBorder.width, 150.0f, 0.0f, (Vector2){0.0f, 30.0f}, (Rectangle){0.0f, 0.0f, (float) frogpng.width, (float) frogpng.height});
    frogs.push_back(tempfrog);

    spiderpng = LoadTexture("assets/Spider.png");
    spiderwebpng = LoadTexture("assets/SpiderWeb.png");
    tempspiderweb = makeObject(spiderwebpng, RightBorder.x, 200.0f, 0.0f, (Vector2){(float) spiderpng.width, 0.0f}, (Rectangle){0.0f, 0.0f, (float) spiderpng.width, (float) spiderpng.height});
    tempspider = makeObject(spiderpng, tempspiderweb.position.x - tempspiderweb.position.width/2.0f, tempspiderweb.position.y + tempspiderweb.position.height/2.0f, 0.0f, (Vector2){(float) spiderpng.width/2.0f, spiderpng.height/2.0f}, (Rectangle){0.0f, 0.0f, (float) spiderpng.width, (float) spiderpng.height});
    spiders.push_back(tempspider);
    flycounter = 0;
    flyframe = 0;
    camera = {0};
    camera.target = (Vector2){fly.position.x, fly.position.y};
    camera.offset = (Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    gamebackgroundinfo = {(Rectangle) {0.0f, 0.0f, (float) gamebackground.width, (float) gamebackground.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
}

void UpdateGame(void) {
    Animations();
    Movement();
   // camera.zoom = GetScreenWidth()/800.0f;
    camera.target = (Vector2){fly.position.x, fly.position.y};

    for (int i = 0; i < (int) flyswatters.size(); i++) {
        flyswatters[i].rotation += FLYSWATTERROTATION;
    }

    for (int i = 0; i < (int) frogs.size(); i++) {
        if (std::string(frogs[i].direction) == "forward") {
            frogs[i].tongue.width += TONGUESPEED;
        } else {
            frogs[i].tongue.width -= TONGUESPEED;
        }

        if (frogs[i].tongue.width > 100) {
            frogs[i].direction = "backward";
        } else if (frogs[i].tongue.width < 10) {
            frogs[i].direction = "forward";
        }
    }
    for (int i = 0; i < (int) spiders.size(); i++) {
        spiders[i].rotation += FLYSWATTERROTATION;
    }
}

void DrawGame(void) {
    BeginMode2D(camera);
        DrawTextureNPatch(gamebackground, gamebackgroundinfo, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE);
        DrawRectangleRec(LeftBorder, SKYBLUE);
        DrawRectangleRec(RightBorder, SKYBLUE);
        //DrawTexture(gamebackground, GetScreenWidth()/2.0f, GetScreenHeight()/2.0f, RAYWHITE);
        DrawTexturePro(flypng, fly.draw, fly.position, fly.origin, fly.rotation, RAYWHITE);
        DrawCircleLines(fly.position.x, fly.position.y, fly.position.width/2.0f, BLUE);

        for (int i = 0; i < (int) flyswatters.size(); i++) {
            DrawTexturePro(flyswatterpng, flyswatters[i].draw, flyswatters[i].position, flyswatters[i].origin, flyswatters[i].rotation, RAYWHITE);
        }

        for (int i = 0; i < (int) frogs.size(); i++) {
            DrawTexturePro(frogpng, frogs[i].draw, frogs[i].position, frogs[i].origin, frogs[i].rotation, RAYWHITE);
            DrawRectangleRec(frogs[i].tongue, PINK);
        }

        DrawTexturePro(spiderwebpng, tempspiderweb.draw, tempspiderweb.position, tempspiderweb.origin, tempspiderweb.rotation, RAYWHITE);
        for (int i = 0; i < (int) spiders.size(); i++) {
            DrawTexturePro(spiderpng, spiders[i].draw, spiders[i].position, spiders[i].origin, spiders[i].rotation, RAYWHITE);
        }
       
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
    UnloadTexture(flyswatterpng);
    UnloadTexture(frogpng);
    UnloadTexture(spiderpng);
}