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

#include <utility>

using std::swap;
using std::max;
using std::min;

bool CheckBorders(mainHero* Laplas, SDL_Rect* item)
{


	return 1;
}

int main(int argc, char* argv[])
{
	int bordersCount;
	FILE* f;
	SDL_Rect* bordersHitbox;
	int check = 1;
	
	fopen_s(&f, "Borders.txt", "r");
	fscanf_s(f, "%d", &bordersCount);

	bordersHitbox = (SDL_Rect*)malloc(sizeof(SDL_Rect) * bordersCount);

	for (int i = 0;i < bordersCount;i++)
	{
		fscanf_s(f, "%d", &bordersHitbox[i].x);
		fscanf_s(f, "%d", &bordersHitbox[i].y);
		fscanf_s(f, "%d", &bordersHitbox[i].w);
		fscanf_s(f, "%d", &bordersHitbox[i].h);
	}

	mainHero Laplas = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, HERO_WIDHT, HERO_HEIGHT }, 
		{X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, SPEED, GRAVITY, ACCELERATION, IMPULSE, ON_BORDER} };
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
					Laplas.physic.xMoveL = -1;
					Laplas.physic.gazeDirection = 0;
					break;
				case SDL_SCANCODE_D:
					Laplas.physic.xMoveR = 1;
					Laplas.physic.gazeDirection = 1;
					break;
				case SDL_SCANCODE_SPACE:
					if (Laplas.physic.onBorder)
					{
						Laplas.physic.impulse = 0.8;
						Laplas.physic.acceleration = 0;
					}
					break;
				}
				break;

				

			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_A:
					Laplas.physic.xMoveL = 0;
					break;
				//case SDL_SCANCODE_W:
				//	Laplas.physic.yMove = 0;
				//	break;
				//case SDL_SCANCODE_S:
				//	Laplas.physic.yMove = 0;
				//	break;
				case SDL_SCANCODE_D:
					Laplas.physic.xMoveR = 0;
					break;
				}
				break;
			}
			

		}
		#pragma endregion

		#pragma region PHYSIC_CHECK

		//Получение координат
		Laplas.xPosition = Laplas.hitbox.x;
		Laplas.yPosition = Laplas.hitbox.y;
		Laplas.physic.gazeDirection = Laplas.physic.xMoveL + Laplas.physic.xMoveR;

		//Движение по оси X
		Laplas.xPosition +=  (Laplas.physic.xMoveL + Laplas.physic.xMoveR) * Laplas.physic.speed;

		//Прыжок
		if (Laplas.physic.impulse > 0.1)
		{
			Laplas.yPosition -= 30 * Laplas.physic.impulse;
			if (Laplas.physic.impulse > 0.1)
				Laplas.physic.impulse -= 0.02;
			Laplas.physic.onBorder = 0;
		}

		//Гравитация
		Laplas.yPosition += Laplas.physic.gravity * Laplas.physic.acceleration;
		if (Laplas.physic.acceleration < 1)
			Laplas.physic.acceleration += 0.05;
		//---------------------------------------------Добавить обнуление ускорения

		//Проверка на наложение хитбоксов
		check = 1;
		for (int i = 0;i < bordersCount;i++)
		{
			if (!CheckBorders(&Laplas, &bordersHitbox[i]))
			{
				Laplas.physic.onBorder = 1;
				check = 0;
				break;
			}
		}
		if (check == 1)
		{
			Laplas.hitbox.x = Laplas.xPosition;
			Laplas.hitbox.y = Laplas.yPosition;
		}



		#pragma endregion 

		#pragma region DRAW
		SDL_SetRenderDrawColor(ren, 255, 0, 128, 255);
		SDL_RenderFillRect(ren, &Laplas.hitbox);



		SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
		for (int i = 0;i < bordersCount;i++)
		{
			SDL_RenderFillRect(ren, &bordersHitbox[i]);
		}

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


//SDL_Surface* surface = IMG_Load("barrel1.png");
//
//SDL_Rect destrect = { Laplas.hitbox.x, Laplas.hitbox.y, 100, 100 };
//
//SDL_BlitSurface(surface, NULL, win_surface, &destrect);
//
//SDL_UpdateWindowSurface(win);