#ifndef SCREENS_H
#define SCREENS_H

enum GameScreen {LOGO, ANIM, GAME};

// Main Functions Initializing
void UpdateDrawingFrame(void);
void InitMain(void);
void UnloadMain(void);

// Game Functions Initializing
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void UnloadGame(void);

#endif