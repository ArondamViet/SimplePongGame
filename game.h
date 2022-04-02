#ifndef _GAME_H
#define _GAME_H 1
//windows/directx headers
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <dsound.h>
#include <dinput.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
//framework-specific headers
#include "dxgraphics.h"
//#include "dxaudio.h"
#include "dxinput.h"
//application title
#define APPTITLE "Paddle_Game"
//screen setup
#define FULLSCREEN 0 //0 = windowed, 1 = fullscreen
#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720
//function prototypes
int Game_Init(HWND);
void Game_Run(HWND);
void Game_End(HWND);
//sprite structure
typedef struct {
	int x, y;
	int width, height;
	int movex, movey;
	int curframe, lastframe;
	int animdelay, animcount;
	int scalex, scaley;
	int rotation, rotaterate;
} SPRITE;
#endif