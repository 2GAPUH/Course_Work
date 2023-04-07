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

bool CheckVectorCross(SDL_Point firstStart, SDL_Point firstEnd, SDL_Point secondStart, SDL_Point secondEnd)
{
	SDL_Point vektor1 = { firstEnd.x - firstStart.x, firstEnd.y - firstStart.y };
	SDL_Point vektor2 = { secondEnd.x - secondStart.x, secondEnd.y - secondStart.y };

	SDL_Point prod1 = { vektor1.x * (secondStart.x - firstStart.x), vektor1.y * (secondStart.y - firstStart.y) };
	SDL_Point prod2 = { vektor1.x * (secondEnd.x - firstStart.x), vektor1.y * (secondEnd.y - firstStart.y) };

	if (vektor1.x == 0 && vektor2.y == 0)
	{
		if ((firstStart.y < firstEnd.y && firstEnd.y >= secondEnd.y && firstStart.y <= secondEnd.y) || (firstStart.y > firstEnd.y && firstStart.y >= secondEnd.y && firstEnd.y <= secondEnd.y))
			return 1;
	}
	else if (vektor1.y == 0 && vektor2.x == 0)
	{
		if ((firstStart.x < firstEnd.x && firstEnd.x >= secondEnd.x && firstStart.x <= secondEnd.x) || (firstStart.x > firstEnd.x && firstStart.x >= secondEnd.x && firstEnd.x <= secondEnd.x))
			return 1;
	}

	if (((prod1.x == prod2.x) && (prod1.y == prod2.y)) || (prod1.x == prod1.y == 0) || (prod2.x ==  prod2.y ))
		return 0;

	prod1 = { vektor2.x * (firstStart.x - secondStart.x), vektor2.y * (firstStart.y - secondStart.y) };
	prod2 = { vektor2.x * (firstEnd.x - secondStart.x), vektor2.y * (firstEnd.y - secondStart.y) };

	if (((prod1.x == prod2.x) && (prod1.y == prod2.y)) || (prod1.x == prod1.y ) || (prod2.x ==  prod2.y ))
		return 0;

	return 1;
};


bool ChechBordersOverlay(mainHero* Laplas, SDL_Rect* item, int type)
{
	switch (type)
	{
	//прямая x0:0 y1:0
	case 1:	
		if (CheckVectorCross({ Laplas->hitbox.x, Laplas->hitbox.y + Laplas->hitbox.h }, { Laplas->xPosition , Laplas->yPosition + Laplas->hitbox.h },
			{item->x, item->y}, {item->x+item->w, item->y}))
				return 1;
		break;

	//прямая x0:0 y0:1
	case 2:
		if (CheckVectorCross({ Laplas->hitbox.x + Laplas->hitbox.w, Laplas->hitbox.y }, { Laplas->xPosition + Laplas->hitbox.w, Laplas->yPosition },
			{ item->x, item->y }, { item->x, item->y + item->h }))
				return 1;
		break;

	//прямая x1:0 y1:1
	case 3:
		if (CheckVectorCross({ Laplas->hitbox.x, Laplas->hitbox.y }, { Laplas->xPosition, Laplas->yPosition }, 
			{ item->x + item->w, item->y }, { item->x + item->w, item->y + item->h }))
				return 1;
		break;

	//прямая x0:1 y1:1
	case 4:
		if(CheckVectorCross({ Laplas->hitbox.x, Laplas->hitbox.y }, { Laplas->xPosition, Laplas->yPosition }, 
			{ item->x , item->y + item->h }, { item->x + item->w, item->y + item->h }))
				return 1;
		break;

	}
	return 0;
}

bool CheckBorders(mainHero* Laplas, SDL_Rect* item)
{
	//Когда тело стоит и падает
	if((Laplas->physic.xMoveL + Laplas->physic.xMoveR == 0) && Laplas->yPosition > Laplas->hitbox.y)
	{
		if (ChechBordersOverlay(Laplas, item, 1))
		{
			Laplas->hitbox.y = item->y - Laplas->hitbox.h;
			return 0;
		}
	}

	//Когда тело стоит и поднимается
	else if ((Laplas->physic.xMoveL + Laplas->physic.xMoveR == 0) && Laplas->yPosition < Laplas->hitbox.y)
	{
		if (ChechBordersOverlay(Laplas, item, 4))
		{
			Laplas->hitbox.y = item->y + item->h;
			return 0;
		}
	}

	//Когда тело двигается в право и падает
	else if ((Laplas->physic.xMoveL + Laplas->physic.xMoveR > 0) && Laplas->yPosition > Laplas->hitbox.y)
	{
		if (ChechBordersOverlay(Laplas, item, 1))
		{
			Laplas->hitbox.y = item->y - Laplas->hitbox.h;
			return 0;
		}
		if (ChechBordersOverlay(Laplas, item, 2))
		{
			Laplas->hitbox.x = item->x;
			return 0;
		}
	}

	//Когда тело двигается в право и поднимается
	else if ((Laplas->physic.xMoveL + Laplas->physic.xMoveR > 0) && Laplas->yPosition < Laplas->hitbox.y)
	{
		if (ChechBordersOverlay(Laplas, item, 4))
		{
			Laplas->hitbox.y = item->y + item->h;
			return 0;
		}

		if (ChechBordersOverlay(Laplas, item, 2))
		{
			Laplas->hitbox.x = item->x;
			return 0;
		}
	}

	//Когда тело двигается в лево и падает
	else if ((Laplas->physic.xMoveL + Laplas->physic.xMoveR < 0) && Laplas->yPosition > Laplas->hitbox.y)
	{
		if (ChechBordersOverlay(Laplas, item, 1))
		{
			Laplas->hitbox.y = item->y - Laplas->hitbox.h;
			return 0;
		}

		if (ChechBordersOverlay(Laplas, item, 3))
		{
			Laplas->hitbox.x = item->x + item->w;
			return 0;
		}
	}

	//Когда тело двигается в лево и поднимается
	else if ((Laplas->physic.xMoveL + Laplas->physic.xMoveR < 0) && Laplas->yPosition < Laplas->hitbox.y)
	{
		if (ChechBordersOverlay(Laplas, item, 3))
		{
			Laplas->hitbox.x = item->x + item->w;
			return 0;
		}
		if (ChechBordersOverlay(Laplas, item, 4))
		{
			Laplas->hitbox.y = item->y + item->h;
			return 0;
		}
	}

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