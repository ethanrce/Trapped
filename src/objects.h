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
};

Object makeObject(Texture2D png, float x, float y, float rot, Vector2 origin, Rectangle draw);
void Animations(void);
void Movement(void);
bool CheckCollisions(void);
#endif