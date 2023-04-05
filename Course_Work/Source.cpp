#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include "func.h"
#include "common_parameters.h"
#include <math.h>

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event ev;


bool CheckBorders(SDL_Rect* unit, SDL_Rect* item, int Move)
{
	if (unit->x > item->x && unit->x < item->x + item->w)
		if (unit->y > item->y && unit->y < item->y + item->h)
		{
			unit->y = item->y+item->h;
			return 0;
		}

	if (unit->x > item->x && unit->x < item->x + item->w)
		if (unit->y + unit->h > item->y && unit->y + unit->h < item->y + item->h)
		{

			unit->y = item->y - unit->h;

			return 0;
		}

	if (unit->x + unit->w> item->x && unit->x + unit->w < item->x + item->w)
		if (unit->y > item->y && unit->y < item->y + item->h)
		{

			unit->y = item->y + item->h;
			return 0;
		}

	if (unit->x + unit->w > item->x && unit->x + unit->w < item->x + item->w)
		if (unit->y + unit->h > item->y && unit->y + unit->h < item->y + item->h)
		{

			unit->y = item->y - unit->h;

			return 0;
		}

	return 1;
}

int main(int argc, char* argv[])
{
	SDL_Rect borders{ 300, 350, WINDOW_WIDTH- 600, 300 };


	mainHero Laplas = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, HERO_WIDHT, HERO_HEIGHT }, 
		{X_MOVE, Y_MOVE, GAZE_DIRECTION, SPEED, GRAVITY, ACCELERATION, IMPULSE, ON_BORDER} };
	int temp = 0;
	int time = 0;

	Init(&win, &ren, &win_surface, WINDOW_HEIGHT, WINDOW_WIDTH);

	bool isRunning = true;

	while (isRunning)
	{
		#pragma region BUTTON_CHECK
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;

			case SDL_KEYDOWN:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					isRunning = false;
					break;
				case SDL_SCANCODE_A:
					Laplas.physic.xMove = -1;
					Laplas.physic.gazeDirection = 0;
					break;
				//case SDL_SCANCODE_W:
				//	Laplas.physic.yMove = -1;
				//	break;
				//case SDL_SCANCODE_S:
				//	Laplas.physic.yMove = 1;
				//	break;
				case SDL_SCANCODE_D:
					Laplas.physic.xMove = 1;
					Laplas.physic.gazeDirection = 1;
					break;
				case SDL_SCANCODE_SPACE:
					if (Laplas.physic.onBorder)
					{
						Laplas.physic.impulse = 1;
						Laplas.physic.acceleration = 0;
					}
					break;
				}
				break;

				

			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_A:
					Laplas.physic.xMove = 0;
					break;
				//case SDL_SCANCODE_W:
				//	Laplas.physic.yMove = 0;
				//	break;
				//case SDL_SCANCODE_S:
				//	Laplas.physic.yMove = 0;
				//	break;
				case SDL_SCANCODE_D:
					Laplas.physic.xMove = 0;
					break;
				}
				break;
			}
			

		}
		#pragma endregion

		#pragma region PHYSIC_CHECK

		if (Laplas.hitbox.x + (temp = Laplas.physic.xMove * Laplas.physic.speed) >= 0 && Laplas.physic.xMove == -1 )
			Laplas.hitbox.x += temp;							  
																  
		if (Laplas.hitbox.x + (temp = Laplas.physic.xMove * Laplas.physic.speed) + HERO_WIDHT <= WINDOW_WIDTH && Laplas.physic.xMove == 1)
			Laplas.hitbox.x += temp;							  
																  
		if (Laplas.hitbox.y + Laplas.physic.gravity + HERO_HEIGHT <= WINDOW_HEIGHT)
		{
			Laplas.hitbox.y += Laplas.physic.gravity * Laplas.physic.acceleration;
			if (Laplas.physic.acceleration < 1)
				Laplas.physic.acceleration += 0.05;
		}
		else
		{
			Laplas.physic.acceleration = 0;
			Laplas.physic.onBorder = 1;
		}


		if (Laplas.physic.impulse > 0)
		{
			Laplas.hitbox.y -= 30 * Laplas.physic.impulse;
			if (Laplas.physic.impulse > 0)
				Laplas.physic.impulse -= 0.05;
			Laplas.physic.onBorder = 0;
		}
			
		if(!CheckBorders(&Laplas.hitbox, &borders, Laplas.physic.yMove))
			Laplas.physic.onBorder = 1;


		#pragma endregion 

		#pragma region DRAW
		SDL_SetRenderDrawColor(ren, 255, 0, 128, 255);
		SDL_RenderFillRect(ren, &Laplas.hitbox);

		SDL_SetRenderDrawColor(ren, 128, 128, 128, 255);
		SDL_RenderFillRect(ren, &borders);

		SDL_RenderPresent(ren);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
		#pragma endregion 

		#pragma region FPS_DELAY
				while (true)
				{
					if (clock() - time >= 1000/FPS)
					{
						time = clock();
						break;
					}
					else
						SDL_Delay(1);
				}
		#pragma endregion 
	}

	DeInit(0, &win, &ren, &win_surface);
	
	return 0;
}


//SDL_Surface* surface = IMG_Load("Hentai.jpg");
//
//SDL_Rect destrect = { 0, 0, 100, 100 };
//
//SDL_BlitSurface(surface, NULL, win_surface, &destrect);
//
//SDL_UpdateWindowSurface(win);