#ifndef SCREENS_H
#define SCREENS_H

enum GameScreen {LOGO, MAINMENU, GAME};

//----------------------------------------------------------------------------------
// Main Functions Initializing
//----------------------------------------------------------------------------------
void UpdateDrawingFrame(void);
void InitMain(void);
void UnloadMain(void);

//----------------------------------------------------------------------------------
// Logo Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitLogoScreen(void);
void UpdateLogoScreen(void);
void DrawLogoScreen(void);
void UnloadLogoScreen(void);
int FinishLogoScreen(void);

//----------------------------------------------------------------------------------
// MainMenu Screen Functions Declaration
//----------------------------------------------------------------------------------
void InitMainMenu(void);
void UpdateMainMenu(void);
void DrawMainMenu(void);
void UnloadMainMenu();
bool FinishMainMenu(void);

//----------------------------------------------------------------------------------
// Game Functions Initializing
//----------------------------------------------------------------------------------
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void UnloadGame(void);
bool FinishGame(void);
#endif