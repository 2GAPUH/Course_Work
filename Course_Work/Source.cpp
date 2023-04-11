#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include "func.h"
#include "common_parameters.h"
#include <math.h>
#include "Physic.h"

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event ev;

void DrawMainHero(mainHero Laplas)
{
	SDL_SetRenderDrawColor(ren, 255, 0, 128, 255);
	SDL_Rect movedLaplas = { Laplas.hitbox.x - Laplas.hitbox.w / 2,Laplas.hitbox.y - Laplas.hitbox.h / 2, Laplas.hitbox.w, Laplas.hitbox.h };
	SDL_RenderFillRect(ren, &movedLaplas);
}

void DrawHitbox(int bordersCount, mainBorders levelBorders[])
{
	SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
	for (int i = 0;i < bordersCount;i++)
	{
		if(levelBorders[i].type == 2)
			SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
		else if (levelBorders[i].type == 3)
			SDL_SetRenderDrawColor(ren, 255, 128, 128, 255);
		SDL_RenderFillRect(ren, &levelBorders[i].bordersHitbox);
	}
}

void FPSControl()
{
	static int time = 0;
	while (true)
	{
		if (clock() - time >= 1000 / FPS)
		{
			time = clock();
			break;
		}
		else
			SDL_Delay(1);
	}
}

mainBorders* LoadLevel(mainBorders* levelBorders, int *bordersCount, mainHero* Laplas, const char levelName[])
{
	FILE* f;
	fopen_s(&f, levelName, "r");

	fscanf_s(f, "%d", bordersCount);
	fscanf_s(f, "%d", &Laplas->hitbox.x);
	fscanf_s(f, "%d", &Laplas->hitbox.y);

	levelBorders = (mainBorders*)realloc(levelBorders, sizeof(mainBorders) * (* bordersCount));

	for (int i = 0;i < *bordersCount;i++)
	{
		fscanf_s(f, "%d", &levelBorders[i].type);
		fscanf_s(f, "%d", &levelBorders[i].bordersHitbox.x);
		fscanf_s(f, "%d", &levelBorders[i].bordersHitbox.y);
		fscanf_s(f, "%d", &levelBorders[i].bordersHitbox.w);
		fscanf_s(f, "%d", &levelBorders[i].bordersHitbox.h);
	}


	fclose(f);

	return levelBorders;
}

bool PhysicInRange(SDL_Point unit, SDL_Rect bordersHitbox)
{
	if (unit.x > bordersHitbox.x && unit.x < bordersHitbox.x + bordersHitbox.w)
		if (unit.y > bordersHitbox.y && unit.y < bordersHitbox.y + bordersHitbox.h)
			return 1;
	return 0;
}


int main(int argc, char* argv[])
{
	int bordersCount;
	mainBorders* levelBorders = NULL;
	int check = 1;
	int temp = 0;
	windowSize window = { WINDOW_WIDTH ,WINDOW_HEIGHT };

	mainHero Laplas = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, HERO_WIDHT, HERO_HEIGHT },
				  {X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER},
				  {DASH_CD, CAMERA_SCALE_X, CAMERA_SCALE_Y} };
	
	
	levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Borders.txt");

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

			case SDL_WINDOWEVENT:
				switch (ev.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					SDL_GetWindowSize(win, &window.w, &window.h);
					SDL_RenderSetScale(ren, window.w / 1. / WINDOW_WIDTH, window.h / 1. / WINDOW_HEIGHT);
				}
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
				case SDL_SCANCODE_LSHIFT:
					if (clock() - Laplas.effect.timeCD > Laplas.effect.dashCD && Laplas.physic.gazeDirection != 0)
					{
						Laplas.effect.timeCD = clock();
						Laplas.physic.accelerationX = 8;
					}
					break;
				case SDL_SCANCODE_SPACE:
					if (Laplas.physic.onBorder)
					{
						Laplas.physic.impulse = 0.8;
						Laplas.physic.accelerationY = 0;
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

			case SDL_MOUSEBUTTONDOWN:
				if (ev.button.button == SDL_BUTTON_X2)
				{
					if (clock() - Laplas.effect.timeCD > Laplas.effect.dashCD && Laplas.physic.gazeDirection !=0)
					{
						Laplas.effect.timeCD = clock();
						Laplas.physic.accelerationX = 8;
					}
				}
				break;

			}



		}
		#pragma endregion

		#pragma region PHYSIC_CHECK

		//Получение координат
		PhysicGetBase(&Laplas);

		//Движение по оси X + рывок
		PhysicXmovement(&Laplas);

		//Прыжок
		PhysicJump(&Laplas);

		//Гравитация
		PhysicGravity(&Laplas);

		//Проверка на наложение хитбоксов
		PhysicHitboxOverlay(bordersCount, &Laplas, levelBorders);

		//Выход за границы мира
		PhysicOutworldCheck(&Laplas, levelBorders);

		if (PhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, levelBorders[7].bordersHitbox))
			levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Borders1.txt");

		#pragma endregion 

		#pragma region DRAW
		DrawMainHero(Laplas);

		DrawHitbox(bordersCount, levelBorders);

		SDL_RenderPresent(ren);

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
		#pragma endregion 

		#pragma region FPS_DELAY

		FPSControl();

		#pragma endregion 
	}

	free(levelBorders);

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