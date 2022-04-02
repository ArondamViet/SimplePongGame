#include "game.h"
//background image
LPDIRECT3DSURFACE9 back;
//sprite handler
LPD3DXSPRITE sprite_handler;
//ball sprite
LPDIRECT3DTEXTURE9 ball_image;
SPRITE ball;
//paddle sprite
LPDIRECT3DTEXTURE9 paddle_image;
SPRITE paddle;

LPDIRECT3DTEXTURE9 e_paddle_image;
SPRITE e_paddle;


//the wave sound
//CSound* sound_bounce;
//CSound* sound_hit;


//misc
long start = GetTickCount();
HRESULT result;
//initializes the game
int Game_Init(HWND hwnd)
{
	//set random number seed
	srand(time(NULL));
	//initialize mouse
	if (!Init_Mouse(hwnd))
	{
		MessageBox(hwnd, "Error initializing the mouse", "Error", MB_OK);
			return 0;
	}
	//initialize keyboard
	if (!Init_Keyboard(hwnd))
	{
		MessageBox(hwnd, "Error initializing the keyboard", "Error", MB_OK);
			return 0;
	}
	//create sprite handler object
	result = D3DXCreateSprite(d3ddev, &sprite_handler);
	if (result != D3D_OK)
		return 0;

	//load the background image
	back = LoadSurface((char*)"space_back.png", NULL);
	if (back == NULL)
		return 0;

	//load the ball sprite
	ball_image = LoadTexture((char*)"Earth_Ball.png", D3DCOLOR_XRGB(0, 0, 0));
	if (ball_image == NULL)
		return 0;
	//set the ball's properties
	ball.x = 400;
	ball.y = 200;
	ball.width = 64;
	ball.height = 64;
	ball.curframe = 0;
	ball.lastframe = 256;
	ball.animdelay = 1;
	ball.animcount = 0;
	ball.movex = 8;
	ball.movey = -8;

	//load the paddle sprite
	paddle_image = LoadTexture((char*)"paddle.png", D3DCOLOR_XRGB(255, 0, 255));
	if (paddle_image == NULL)
		return 0;
	//set paddle properties
	paddle.x = 300;
	paddle.y = SCREEN_HEIGHT - 50;
	paddle.width = 171;
	paddle.height = 58;
	


	//load the paddle sprite
	e_paddle_image = LoadTexture((char*)"e_paddle.png", D3DCOLOR_XRGB(255, 0, 255));
	if (e_paddle_image == NULL)
		return 0;
	//set paddle properties
	e_paddle.x = 300;
	e_paddle.y = 0 ;
	e_paddle.width = 171;
	e_paddle.height = 58;


	//
	////load bounce wave file
	//sound_bounce = LoadSound("bounce.wav");
	//if (sound_bounce == NULL)
	//	return 0;
	////load the hit wave file
	//sound_hit = LoadSound("hit.wav");
	//if (sound_hit == NULL)
	//	return 0;
	////return okay


	return 1;
}
int Collision(SPRITE sprite1, SPRITE sprite2)
{
	RECT rect1;
	rect1.left = sprite1.x + 1;
	rect1.top = sprite1.y + 1;
	rect1.right = sprite1.x + sprite1.width - 1;
	rect1.bottom = sprite1.y + sprite1.height - 1;
	RECT rect2;
	rect2.left = sprite2.x + 1;
	rect2.top = sprite2.y + 1;
	rect2.right = sprite2.x + sprite2.width - 1;
	rect2.bottom = sprite2.y + sprite2.height - 1;
	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}
//the main game loop
void Game_Run(HWND hwnd)
{
	//ball position vector
	D3DXVECTOR3 position(0, 0, 0);
	//make sure the Direct3D device is valid
	if (d3ddev == NULL)
		return;
	//update mouse and keyboard
	Poll_Mouse();
	Poll_Keyboard();
	//after short delay, ready for next frame?
	//this keeps the game running at a steady frame	rate
		if (GetTickCount() - start >= 30)
		{
			//reset timing
			start = GetTickCount();
			//move the ball sprite
			ball.x += ball.movex;
			ball.y += ball.movey;
			//bounce the ball at screen edges
			if (ball.x > SCREEN_WIDTH - ball.width)
			{
				ball.x -= ball.width;
				ball.movex *= -1;				
			}
			else if (ball.x < 0)
			{
				ball.x += ball.width;
				ball.movex *= -1;				
			}
			if (ball.y > SCREEN_HEIGHT - ball.height)
			{
				ball.y -= ball.height;
				ball.movey *= -1;				
			}
			else if (ball.y < 0)
			{
				ball.y += ball.height;
				ball.movey *= -1;
			}

			if (++ball.animcount > ball.animdelay) {
				ball.animcount = 0;
				if (++ball.curframe > ball.lastframe)
					ball.curframe = 0;
			}


			//move the paddle
			//paddle.x += Mouse_X();
			if (paddle.x > SCREEN_WIDTH - paddle.width)
				paddle.x = SCREEN_WIDTH - paddle.width;
			else if (paddle.x < 0)
				paddle.x = 0;
			//check for left arrow
			if (Key_Down(DIK_LEFT))
				paddle.x -= 20;
			//check for right arrow
			if (Key_Down(DIK_RIGHT))
				paddle.x += 20;
			//see if ball hit the paddle
			if (Collision(paddle, ball)){
				ball.y -= ball.movey;
				ball.movey *= -1;
				
			}


			//move the paddle
			//e_paddle.x += Mouse_X();
			if (e_paddle.x > SCREEN_WIDTH - e_paddle.width)
				e_paddle.x = SCREEN_WIDTH - e_paddle.width;
			else if (e_paddle.x < 0)
				e_paddle.x = 0;
			//check for left arrow
			if (Key_Down(DIK_A))
				e_paddle.x -= 20;
			//check for right arrow
			if (Key_Down(DIK_D))
				e_paddle.x += 20;
			//see if ball hit the paddle
			if (Collision(e_paddle, ball)){
				ball.y -= ball.movey;
				ball.movey *= -1;
				
			}
		}

	//start rendering
	if (d3ddev->BeginScene()){

		//erase the entire background
		d3ddev->StretchRect(back, NULL, backbuffer,	NULL, D3DTEXF_NONE);
		//start sprite handler
		sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

		RECT srcRect;
		int columns = 16;




		//draw the ball
		position.x = (float)ball.x;
		position.y = (float)ball.y;

		srcRect.left = (ball.curframe % columns) * ball.width;
		srcRect.top = (ball.curframe / columns) * ball.height;
		srcRect.right = srcRect.left + ball.width;
		srcRect.bottom = srcRect.top + ball.height;

		sprite_handler->Draw(
			ball_image,
			&srcRect, 
			NULL,
			&position,
			D3DCOLOR_XRGB(255, 255, 255));

		//draw the paddle
		position.x = (float)paddle.x;
		position.y = (float)paddle.y;
		sprite_handler->Draw(
			paddle_image,
			NULL,
			NULL,
			&position,
			D3DCOLOR_XRGB(255, 255, 255));

		position.x = (float)e_paddle.x;
		position.y = (float)e_paddle.y;
		sprite_handler->Draw(
			e_paddle_image,
			NULL,
			NULL,
			&position,
			D3DCOLOR_XRGB(255, 255, 255));


		//stop drawing
		sprite_handler->End();

		//stop rendering
		d3ddev->EndScene();
	}
	//display the back buffer on the screen
	d3ddev->Present(NULL, NULL, NULL, NULL);
	//check for mouse button (to exit program)
	if (Mouse_Button(0))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
	//check for escape key (to exit program)
	if (Key_Down(DIK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);
}
//frees memory and cleans up before the game ends
void Game_End(HWND hwnd)
{
	if (ball_image != NULL)
		ball_image->Release();
	if (paddle_image != NULL)
		paddle_image->Release();
	if (back != NULL)
		back->Release();
	if (sprite_handler != NULL)
		sprite_handler->Release();
	//if (sound_bounce != NULL)
	// delete sound_bounce;
	//if (sound_hit != NULL)
	// delete sound_hit;
}