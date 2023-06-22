#ifndef OBJECTS_H
#define OBJECTS_H
#include <raylib.h>

class Object {
    public:
    Texture2D texture;
    Rectangle draw;
    Rectangle position;
    Vector2 origin;
    float rotation;
    Rectangle tongue;
    const char *direction;
    const char *side;
};

Object makeObject(Texture2D png, float x, float y, float rot, Vector2 origin, Rectangle draw, const char *side);
void EnemyMovement(void);
void RotateEnemy(int index);
void MakeEnemies(void);
void Animations(void);
void Movement(void);
bool CheckCollisions(void);
float RandomNum(int min, int max);
#endif