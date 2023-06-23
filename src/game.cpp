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
#define SPIDERROTATION 1.0f
#define FLYSWATTERROTATION 2.0f
#define TONGUESPEED 3.0f
#define SPIDERCOOLDOWN 1.5f
#define SPIDERSHOTSPEED 3.0f
#define ROUNDSIZE 500
#define SPRITESPACE 150
#define FIRSTSPRITE 196
#define STORAGE_DATA_FILE "storage.data"

typedef enum {
    STORAGE_POSITION_HISCORE = 0,
} StorageData;

static bool SaveStorageValue(unsigned int position, int value);
static int LoadStorageValue(unsigned int position);
int flycounter, flyframe;
int spidercounter;
int gameround;
bool gameover;
float rotatedX, rotatedY;
vector<Object> flyswatters, frogs, spidershots, spiderwebs, spiders;
Texture2D flypng, gamebackground, backgroundbottompng, backgroundtoppng, flyswatterpng, spiderpng, spiderwebpng, spidershotpng, leftfrogpng, rightfrogpng;
Object fly, backgroundbottom, backgroundtop, spider, spiderweb, frog, spidershot, flyswatter;
NPatchInfo gamebackgroundinfo;
Rectangle LeftBorder, RightBorder;
Camera2D camera;
enum RoundScreen {ROUNDSTART, INROUND};
RoundScreen GamePhase;

void InitGame(void) {
    // SaveStorageValue(STORAGE_POSITION_HISCORE, 0); // For publishing purposes after testing
    GamePhase = ROUNDSTART;
    gameround = 0;
    gameover = false;
    RightBorder = (Rectangle){611, 1, (float) 4, (float) 1}; // 106 represents the amount of pixels the beginning of the jar is from the bottom of the texture;
    LeftBorder = (Rectangle){253, 1, 4, (float) 1};
    flypng = LoadTexture("assets/Fly.png");
    fly = makeObject(flypng, GetScreenWidth()/2.0f, GetScreenWidth()/2.0f, 0.0f, (Vector2){flypng.width/4.0f, flypng.height/2.0f}, (Rectangle){0, 0, (float) flypng.width/2.0f, (float) flypng.height}, "neither");
    
    gamebackgroundinfo = {(Rectangle) {0.0f, 0.0f, (float) gamebackground.width, (float) gamebackground.height}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
    backgroundbottompng = LoadTexture("assets/backgroundbottom.png");
    backgroundbottom = makeObject(backgroundbottompng, GetScreenWidth()/2.0f, GetScreenHeight(), 0.0f, (Vector2){(float) backgroundbottompng.width/2.0f, (float) backgroundbottompng.height}, (Rectangle){0.0f, 0.0f,(float) backgroundbottompng.width, (float) backgroundbottompng.height}, "neither");
    backgroundtoppng = LoadTexture("assets/backgroundtop.png");
  
    flyswatterpng = LoadTexture("assets/FlySwatter.png");
    leftfrogpng = LoadTexture("assets/LeftFrog.png");
    rightfrogpng = LoadTexture("assets/RightFrog.png");
    spidershotpng = LoadTexture("assets/SpiderShoot.png");
    spiderpng = LoadTexture("assets/Spider.png");
    spiderwebpng = LoadTexture("assets/SpiderWeb.png");
  
    flycounter = 0;
    flyframe = 0;
    camera = {0};
    camera.target = (Vector2){fly.position.x, fly.position.y};
    camera.offset = (Vector2){GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void UpdateGame(void) {
    switch(GamePhase) {
        case(ROUNDSTART): { 
            flyswatters.clear();
            spiders.clear();
            spiderwebs.clear();
            frogs.clear();
            spidershots.clear();
            gameround ++;
            fly.position.x = GetScreenWidth()/2.0f;
            fly.position.y = GetScreenHeight() - 106 - 50;
            RightBorder.y = (float)(GetScreenHeight() - 106 - 100) - (ROUNDSIZE * gameround); //Subtracting an extra 100 pixels to get above the last enemy drawn
            LeftBorder.y = (float)(GetScreenHeight() - 106 - 100) - (ROUNDSIZE * gameround);
            RightBorder.height = (ROUNDSIZE * gameround) + 100;
            LeftBorder.height = (ROUNDSIZE * gameround) + 100;
            MakeEnemies();
            backgroundtop = makeObject(backgroundtoppng, GetScreenWidth()/2.0f, LeftBorder.y, 0.0f, (Vector2){(float) backgroundtoppng.width/2.0f, (float) backgroundtoppng.height}, (Rectangle){0.0f, 0.0f,(float) backgroundtoppng.width, (float) backgroundtoppng.height}, "neither");
            GamePhase = INROUND;
        } break;
        case(INROUND): break;
    }
    Animations();
    Movement();
    camera.target = (Vector2){fly.position.x, fly.position.y};
    EnemyMovement();
    CheckCollisions();
}

void DrawGame(void) {
    ClearBackground(BEIGE);
    const char *round = ("Round: " + std::to_string(gameround)).c_str();
    DrawText(round, 10, 50, 20, GOLD);
    DrawFPS(10, 10);
    BeginMode2D(camera);
        //DrawTextureNPatch(gamebackground, gamebackgroundinfo, (Rectangle) {0.0f, 0.0f, (float) GetScreenWidth(), (float) GetScreenHeight()}, Vector2Zero(), 0.0f, RAYWHITE);
        DrawTexturePro(backgroundbottompng, backgroundbottom.draw, backgroundbottom.position, backgroundbottom.origin, backgroundbottom.rotation, RAYWHITE);
        DrawRectangleRec(LeftBorder, SKYBLUE); 
        DrawRectangleRec(RightBorder, SKYBLUE);
        DrawTexturePro(backgroundtoppng, backgroundtop.draw, backgroundtop.position, backgroundtop.origin, backgroundtop.rotation, RAYWHITE);
        DrawTexturePro(flypng, fly.draw, fly.position, fly.origin, fly.rotation, RAYWHITE);

        for (int i = 0; i < (int) flyswatters.size(); i++) {
            DrawTexturePro(flyswatters[i].texture, flyswatters[i].draw, flyswatters[i].position, flyswatters[i].origin, flyswatters[i].rotation, RAYWHITE);
        }

        for (int i = 0; i < (int) frogs.size(); i++) {
            DrawTexturePro(frogs[i].texture, frogs[i].draw, frogs[i].position, frogs[i].origin, frogs[i].rotation, RAYWHITE);
            DrawRectangleRec(frogs[i].tongue, PINK);
        }
        
        for (int i = 0; i < (int) spiders.size(); i++) {
            DrawTexturePro(spiderwebpng, spiderwebs[i].draw, spiderwebs[i].position, spiderwebs[i].origin, spiderwebs[i].rotation, RAYWHITE);
            DrawTexturePro(spiderpng, spiders[i].draw, spiders[i].position, spiders[i].origin, spiders[i].rotation, RAYWHITE);
        }

        for (int i = 0; i < (int) spidershots.size(); i++) {
            DrawTexturePro(spidershotpng, spidershots[i].draw, spidershots[i].position, spidershots[i].origin, spidershots[i].rotation, RAYWHITE);
        }
    EndMode2D();
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

    spidercounter ++;
    if (spidercounter >= (float)(FPS/(1.0f/SPIDERCOOLDOWN))) {
        spidercounter = 0;
        for (int i = 0; i < (int) spiders.size(); i++) {
            spidershot = makeObject(spidershotpng, spiders[i].position.x, spiders[i].position.y, spiders[i].rotation, (Vector2){spidershotpng.width/2.0f, spidershotpng.height/2.0f}, (Rectangle){0.0f, 0.0f, (float) spidershotpng.width, (float) spidershotpng.height}, "neither");
            spidershots.push_back(spidershot);
        } 
    }
}

void Movement(void) {
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        fly.rotation += FLYROTATION;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        fly.rotation -= FLYROTATION;
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

void MakeEnemies(void) {
    for (int i = ((GetScreenHeight() - 106) - FIRSTSPRITE); i > (GetScreenHeight() - 106) - (ROUNDSIZE * gameround); i -= SPRITESPACE) {
        float enemy = RandomNum(1, 3);
        float side = RandomNum(1, 2);
        if (enemy == 1) {
            if (side == 1) {
                flyswatter = makeObject(flyswatterpng, LeftBorder.x + LeftBorder.width, (float) i, 0.0f, (Vector2){(float) flyswatterpng.width/2.0f, (float) flyswatterpng.height}, (Rectangle) {0.0f, 0.0f, (float) flyswatterpng.width, (float) flyswatterpng.height}, "left");
                flyswatters.push_back(flyswatter);
            } else {
                flyswatter = makeObject(flyswatterpng, RightBorder.x, (float) i, 0.0f, (Vector2){ (float) flyswatterpng.width/2.0f, (float) flyswatterpng.height}, (Rectangle) {0.0f, 0.0f, (float) flyswatterpng.width, (float) flyswatterpng.height}, "right");
                flyswatters.push_back(flyswatter);
            }
        } else if (enemy == 2) {
            if (side == 1) {
                frog = makeObject(leftfrogpng, LeftBorder.x + LeftBorder.width, (float) i - (rightfrogpng.height - 30), 0.0f, (Vector2){0.0f, 30.0f}, (Rectangle){0.0f, 0.0f, (float) rightfrogpng.width, (float) leftfrogpng.height}, "left");
                frogs.push_back(frog);
            } else {
                frog = makeObject(rightfrogpng, RightBorder.x, (float) i - (leftfrogpng.height - 30), 0.0f, (Vector2){(float) leftfrogpng.width, 30.0f}, (Rectangle){0.0f, 0.0f, (float) leftfrogpng.width, (float) rightfrogpng.height}, "right");
                frogs.push_back(frog);
            }
        } else if (enemy == 3) {
            if (side == 1) {
                spiderweb = makeObject(spiderwebpng, LeftBorder.x + LeftBorder.width, (float) i - spiderwebpng.height, 0.0f, (Vector2){(float) 0.0f, 0.0f}, (Rectangle){0.0f, 0.0f, (float) spiderpng.width, (float) spiderpng.height}, "left");
                spider = makeObject(spiderpng, spiderweb.position.x + spiderweb.position.width/2.0f, spiderweb.position.y + spiderweb.position.height/2.0f, 0.0f, (Vector2){(float) spiderpng.width/2.0f, spiderpng.height/2.0f}, (Rectangle){0.0f, 0.0f, (float) spiderpng.width, (float) spiderpng.height}, "left");
                spiders.push_back(spider);
                spiderwebs.push_back(spiderweb);
            } else {   
                spiderweb = makeObject(spiderwebpng, RightBorder.x, (float) i - spiderwebpng.height, 0.0f, (Vector2){(float) spiderpng.width, 0.0f}, (Rectangle){0.0f, 0.0f, (float) spiderpng.width, (float) spiderpng.height}, "right");
                spider = makeObject(spiderpng, spiderweb.position.x - spiderweb.position.width/2.0f, spiderweb.position.y + spiderweb.position.height/2.0f, 0.0f, (Vector2){(float) spiderpng.width/2.0f, spiderpng.height/2.0f}, (Rectangle){0.0f, 0.0f, (float) spiderpng.width, (float) spiderpng.height}, "right");
                spiders.push_back(spider);
                spiderwebs.push_back(spiderweb);
            }
        }
    }
}

void EnemyMovement(void) {
    for (int i = 0; i < (int) flyswatters.size(); i++) {
        if (std::string(flyswatters[i].direction) == "forward") {
            if (std::string(flyswatters[i].side)  == "left") {
                flyswatters[i].rotation += FLYSWATTERROTATION;
            } else {
                flyswatters[i].rotation -= FLYSWATTERROTATION;
            }
        } else {
            if (std::string(flyswatters[i].side) == "left") {
                flyswatters[i].rotation -= FLYSWATTERROTATION;
            } else {
                flyswatters[i].rotation += FLYSWATTERROTATION;
            }
        }

        if (std::string(flyswatters[i].side) == "left") {
            if (std::string(flyswatters[i].direction) == "forward" && flyswatters[i].rotation > 180) {
                flyswatters[i].direction = "backward";
            } else if (std::string(flyswatters[i].direction) == "backward" && flyswatters[i].rotation < 0) {
                flyswatters[i].direction = "forward";
            }
        } else {
            if (std::string(flyswatters[i].direction) == "forward" && flyswatters[i].rotation < -180) {
                flyswatters[i].direction = "backward";
            } else if (std::string(flyswatters[i].direction) == "backward" && flyswatters[i].rotation > 0) {
                flyswatters[i].direction = "forward";
            }
        }
    }

    for (int i = 0; i < (int) frogs.size(); i++) {
        if (std::string(frogs[i].direction) == "forward") {
            if (std::string(frogs[i].side) == "left") {
                frogs[i].tongue.width += TONGUESPEED;
            } else {
                frogs[i].tongue.x -= TONGUESPEED;
                frogs[i].tongue.width += TONGUESPEED;
            }
        } else {
            if (std::string(frogs[i].side) == "left") {
               frogs[i].tongue.width -= TONGUESPEED; 
            } else {
                frogs[i].tongue.x += TONGUESPEED;
                frogs[i].tongue.width -= TONGUESPEED;
            }
        }

        if (std::string(frogs[i].side) == "left") {
            if (frogs[i].tongue.width > 200) {
                frogs[i].direction = "backward";
            } else if (frogs[i].tongue.width < 10) {
                frogs[i].direction = "forward";
            }
        } else {
            if (frogs[i].tongue.width > 150) {
                frogs[i].direction = "backward";
            } else if (frogs[i].tongue.width < 5) {
                frogs[i].direction = "forward";
            }
        }
    }

    for (int i = 0; i < (int) spiders.size(); i++) {
        RotateEnemy(i);
    }

    for (int i = 0; i < (int) spidershots.size(); i++) {
        spidershots[i].position.x -= SPIDERSHOTSPEED * sinf(spidershots[i].rotation * (PI / 180.0f));
        spidershots[i].position.y += SPIDERSHOTSPEED * cosf(spidershots[i].rotation * (PI / 180.0f));
    }
}
void RotateEnemy(int index) {
    float rotationAngle = ((std::atan2(fly.position.y - spiders[index].position.y, fly.position.x - spiders[index].position.x) * 180.0)/PI);
    float rotationDiff = rotationAngle - (spiders[index].rotation + 90.0f);
    while (rotationDiff > 180.0f) {
        rotationDiff -= 360.0f;
    }  
    while (rotationDiff < -180.0f) {
        rotationDiff += 360.0f;
    }
    float maxRotationChange = SPIDERROTATION;
    float rotationChange = Clamp(rotationDiff, -maxRotationChange, maxRotationChange);
    spiders[index].rotation += rotationChange;
}

bool CheckCollisions(void) {
    for (int i = 0; i < (int) frogs.size(); i++) {
        if (CheckCollisionCircleRec((Vector2){fly.position.x, fly.position.y}, fly.position.width/2.0f, frogs[i].tongue)) {  
            gameover = true;
            return true;
        }
    }

    for (int i = 0; i < (int) flyswatters.size(); i++) {
        rotatedX = flyswatters[i].position.x + (cosf((flyswatters[i].rotation - 90.0f) * (PI / 180.0f)) * (flyswatters[i].position.height - 15));
        rotatedY = flyswatters[i].position.y + (sinf((flyswatters[i].rotation - 90.0f) * (PI / 180.0f)) * (flyswatters[i].position.height - 15));
        if (CheckCollisionCircles((Vector2){fly.position.x, fly.position.y}, fly.position.width/2.0f, (Vector2){rotatedX, rotatedY}, 25)) {
            gameover = true;
            return true;
        }
    }

    for (int i = 0; i < (int) spidershots.size(); i++) {
        if (CheckCollisionCircles((Vector2){fly.position.x, fly.position.y}, (fly.position.width/2.0f), (Vector2){spidershots[i].position.x, spidershots[i].position.y}, (spidershots[i].position.width/2.0f))) {
            gameover = true;
            return true;
        }
    }
    Rectangle top = (Rectangle) {LeftBorder.x + 13, backgroundtop.position.y - backgroundtop.position.height/2.0f, (RightBorder.x - 16) - (LeftBorder.x + 13), 1};
    if (CheckCollisionCircleRec((Vector2){fly.position.x, fly.position.y}, (fly.position.width/2.0f), top)) {
        GamePhase = ROUNDSTART;
    }
    return false;
}

bool FinishGame(void) {
    if (gameover && gameround > LoadStorageValue(STORAGE_POSITION_HISCORE)) {
        SaveStorageValue(STORAGE_POSITION_HISCORE, gameround);
    }
    return gameover;
} 

void UnloadGame(void) {
    UnloadTexture(flypng);
    UnloadTexture(gamebackground);
    UnloadTexture(backgroundtoppng);
    UnloadTexture(backgroundbottompng);
    UnloadTexture(flyswatterpng);
    UnloadTexture(leftfrogpng);
    UnloadTexture(rightfrogpng);
    UnloadTexture(spiderpng);
}

int GetHighScore(void) {
    return LoadStorageValue(STORAGE_POSITION_HISCORE);
}

bool SaveStorageValue(unsigned int position, int value)
{
    bool success = false;
    unsigned int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL)
    {
        if (dataSize <= (position*sizeof(int)))
        {
            // Increase data size up to position and store value
            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", STORAGE_DATA_FILE, dataSize, position*sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else
        {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(STORAGE_DATA_FILE, newFileData, newDataSize);
        RL_FREE(newFileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }
    else
    {
        TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully", STORAGE_DATA_FILE);

        dataSize = (position + 1)*sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(STORAGE_DATA_FILE, fileData, dataSize);
        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", STORAGE_DATA_FILE, value);
    }

    return success;
}

// Load integer value from storage file (from defined position)
// NOTE: If requested position could not be found, value 0 is returned
int LoadStorageValue(unsigned int position)
{
    int value = 0;
    unsigned int dataSize = 0;
    unsigned char *fileData = LoadFileData(STORAGE_DATA_FILE, &dataSize);

    if (fileData != NULL)
    {
        if (dataSize < (position*4)) TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", STORAGE_DATA_FILE, position);
        else
        {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i", STORAGE_DATA_FILE, value);
    }

    return value;
}