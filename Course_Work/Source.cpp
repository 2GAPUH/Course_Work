#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <iostream>
#include "func.h"
#include "common_parameters.h"
#include <math.h>
#include "HeroPhysic.h"
#include "EnemyPhysic.h"

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Surface* win_surface = NULL;
SDL_Event ev;
int levelWidth = 0, levelHeight = 0;


void DrawMainHero(mainHero Laplas)
{
	SDL_SetRenderDrawColor(ren, 255, 0, 128, 255);
	SDL_Rect movedLaplas = { 640 - Laplas.hitbox.w / 2,360 - Laplas.hitbox.h / 2, Laplas.hitbox.w, Laplas.hitbox.h };

	if (Laplas.hitbox.x < WINDOW_WIDTH / 2)
		movedLaplas.x = Laplas.hitbox.x - Laplas.hitbox.w / 2;
	if(Laplas.hitbox.x > levelWidth - WINDOW_WIDTH / 2)
		movedLaplas.x = Laplas.hitbox.x - Laplas.hitbox.w / 2 - (levelWidth - WINDOW_WIDTH);

	if (Laplas.hitbox.y < WINDOW_HEIGHT / 2)
		movedLaplas.y = Laplas.hitbox.y - Laplas.hitbox.h / 2;
	if (Laplas.hitbox.y > levelHeight - WINDOW_HEIGHT / 2)
		movedLaplas.y = Laplas.hitbox.y - Laplas.hitbox.h / 2 - (levelHeight - WINDOW_HEIGHT);

	SDL_RenderFillRect(ren, &movedLaplas);
}

void DrawHitbox(int bordersCount, mainBorders levelBorders[], mainHero Laplas)
{
	SDL_Rect rect123;
	SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
	for (int i = 0;i < bordersCount;i++)
	{
		switch (levelBorders[i].type)
		{
		case 1:
			SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
			break;

		case 2:
			SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
			break;

		case 3:
			SDL_SetRenderDrawColor(ren, 255, 128, 128, 255);
			break; 

		case 4:
			SDL_SetRenderDrawColor(ren, 200, 200, 0, 255);
			break;
		}

		rect123 = levelBorders[i].bordersHitbox;

		if (Laplas.hitbox.x > WINDOW_WIDTH / 2 && Laplas.hitbox.x < levelWidth - WINDOW_WIDTH / 2)
			rect123.x -= Laplas.hitbox.x - WINDOW_WIDTH / 2;
		if (Laplas.hitbox.x > levelWidth - WINDOW_WIDTH / 2)
			rect123.x -= levelWidth - WINDOW_WIDTH;

		if (Laplas.hitbox.y > WINDOW_HEIGHT / 2 && Laplas.hitbox.y < levelHeight - WINDOW_HEIGHT / 2)
			rect123.y -= Laplas.hitbox.y - WINDOW_HEIGHT / 2;
		if (Laplas.hitbox.y > levelHeight - WINDOW_HEIGHT / 2)
			rect123.y -= levelHeight - WINDOW_HEIGHT;

		SDL_RenderFillRect(ren, &rect123);
	}
}

void DrawEnemys(int enemysCount, mainEnemys levelEnemys[])
{
	SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
	for (int i = 0;i < enemysCount;i++)
	{
		switch (levelEnemys[i].type)
		{
		case 1:
			SDL_SetRenderDrawColor(ren, 129, 0, 255, 255);
			break;
		}

		SDL_Rect movedEnemy = { levelEnemys[i].hitbox.x - levelEnemys[i].hitbox.w / 2,levelEnemys[i].hitbox.y - levelEnemys[i].hitbox.h / 2, levelEnemys[i].hitbox.w, levelEnemys[i].hitbox.h};
		SDL_RenderFillRect(ren, &movedEnemy);

	}
}


mainBorders* LoadLevel(mainBorders* levelBorders, int *bordersCount, mainHero* Laplas, const char levelName[])
{
	FILE* f;
	fopen_s(&f, levelName, "r");

	fscanf_s(f, "%d", bordersCount);
	fscanf_s(f, "%d", &Laplas->hitbox.x);
	fscanf_s(f, "%d", &Laplas->hitbox.y);
	fscanf_s(f, "%d", &levelWidth);
	fscanf_s(f, "%d", &levelHeight);
	
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

mainEnemys* LoadEnemys(mainEnemys* levelEnemys, int* enemysCount, const char levelName[])
{
	FILE* f;
	fopen_s(&f, levelName, "r");

	fscanf_s(f, "%d", enemysCount);

	levelEnemys = (mainEnemys*)realloc(levelEnemys, sizeof(mainEnemys) * (*enemysCount));

	for (int i = 0;i < *enemysCount;i++)
	{
		fscanf_s(f, "%d", &levelEnemys[i].type);
		fscanf_s(f, "%d", &levelEnemys[i].hitbox.x);
		fscanf_s(f, "%d", &levelEnemys[i].hitbox.y);
		fscanf_s(f, "%d", &levelEnemys[i].hitbox.w);
		fscanf_s(f, "%d", &levelEnemys[i].hitbox.h);
	}


	fclose(f);

	return levelEnemys;
}

bool HeroPhysicInRange(SDL_Point unit, SDL_Rect bordersHitbox)
{
	if (unit.x > bordersHitbox.x && unit.x < bordersHitbox.x + bordersHitbox.w)
		if (unit.y > bordersHitbox.y && unit.y < bordersHitbox.y + bordersHitbox.h)
			return 1;
	return 0;
}

mainHero InitHero()
{
	mainHero Laplas;
	return Laplas = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, HERO_WIDHT, HERO_HEIGHT },
			  {X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER},
			  {DASH_CD, CAMERA_SCALE_X, CAMERA_SCALE_Y} };
}

void InitEnemys(mainEnemys levelEnemys[], int enemysCount)
{
	for (int i = 0; i < enemysCount;i++)
	{
		levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
	}
}

int main(int argc, char* argv[])
{
	Init(&win, &ren, &win_surface);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	int bordersCount;
	int enemysCount;
	mainBorders* levelBorders = NULL;
	mainEnemys* levelEnemys = NULL;
	int check = 1;
	mainHero Laplas;
	int temp = 0;
	windowSize window = { WINDOW_WIDTH ,WINDOW_HEIGHT };

	Laplas = InitHero();
	


	levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Borders.txt");
	levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemy.txt");

	InitEnemys(levelEnemys, enemysCount);

	

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
		HeroPhysicGetBase(&Laplas);
		EnemyPhysicGetBase(&levelEnemys[0]);

		//Движение по оси X + рывок
		HeroPhysicXmovement(&Laplas);
		EnemyPhysicXmovement(&levelEnemys[0]);

		//Прыжок
		HeroPhysicJump(&Laplas);
		EnemyPhysicJump(&levelEnemys[0]);

		//Гравитация
		HeroPhysicGravity(&Laplas);
		EnemyPhysicGravity(&levelEnemys[0]);
		
		//Проверка на наложение хитбоксов
		HeroPhysicHitboxOverlay(bordersCount, &Laplas, levelBorders);
		EnemyPhysicHitboxOverlay(bordersCount, &levelEnemys[0], levelBorders);

		//Выход за границы мира
		HeroPhysicOutworldCheck(&Laplas, levelBorders);
		EnemyPhysicOutworldCheck(&levelEnemys[0], levelBorders);

		if (HeroPhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, levelBorders[8].bordersHitbox))
		{
			levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Borders1.txt");
			//levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemy1.txt");
			InitEnemys(levelEnemys, enemysCount);
		}

		#pragma endregion 

		#pragma region DRAW
		DrawMainHero(Laplas);

		DrawHitbox(bordersCount, levelBorders, Laplas);
		DrawEnemys(enemysCount, levelEnemys);

		SDL_RenderPresent(ren);

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
		#pragma endregion 

		#pragma region FPS_DELAY

		FPSControl();

		#pragma endregion 
	}

	free(levelBorders);
	free(levelEnemys);

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