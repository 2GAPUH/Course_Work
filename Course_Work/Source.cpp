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



int Det (int a, int b, int c, int d) 
{
	return a * d - b * c;
}
 
bool Between (int a, int b, double c) 
{
	return min(a,b) <= c + EPS && c <= max(a,b) + EPS;
}
 
bool Intersect (int a, int b, int c, int d) 
{
	if (a > b)  swap (&a, &b);
	if (c > d)  swap (&c, &d);
	return max(a,c) <= min(b,d);
}
 
bool SegmentOverlay (SDL_Point firstStart, SDL_Point firstEnd, SDL_Point secondStart, SDL_Point secondEnd, SDL_Point* intersect) 
{
	int A1 = firstStart.y-firstEnd.y,  B1 = firstEnd.x-firstStart.x,  C1 = -A1*firstStart.x - B1*firstStart.y;
	int A2 = secondStart.y-secondEnd.y,  B2 = secondEnd.x-secondStart.x,  C2 = -A2*secondStart.x - B2*secondStart.y;
	int zn = Det (A1, B1, A2, B2);
	if (zn != 0) {
		double x = - Det (C1, B1, C2, B2) * 1. / zn;
		double y = - Det (A1, C1, A2, C2) * 1. / zn;
		if (Between(firstStart.x, firstEnd.x, x) && Between(firstStart.y, firstEnd.y, y)
			&& Between(secondStart.x, secondEnd.x, x) && Between(secondStart.y, secondEnd.y, y))
		{
			intersect->x = x;
			intersect->y = y;
			return 1;
		}
		else 
			return 0;
	}
	else
		return Det (A1, C1, A2, C2) == 0 && Det (B1, C1, B2, C2) == 0
			&& Intersect (firstStart.x, firstEnd.x, secondStart.x, secondEnd.x)
			&& Intersect (firstStart.y, firstEnd.y, secondStart.y, secondEnd.y);
}


bool CheckBorders(mainHero* Laplas, SDL_Rect unit)
{
	static SDL_Point intersect;
	//Верхнаяя прямая
	if (SegmentOverlay({ Laplas->hitbox.x ,Laplas->hitbox.y + Laplas->hitbox.h/2}, { Laplas->position.x, Laplas->position.y+Laplas->hitbox.h / 2 },
		{ unit.x - Laplas->hitbox.w/2, unit.y }, { unit.x + unit.w + Laplas->hitbox.w / 2, unit.y }, &intersect))
	{
		Laplas->hitbox.x = Laplas->position.x;
		Laplas->hitbox.y = intersect.y - Laplas->hitbox.h/2 - 1;
		Laplas->physic.onBorder = 1;
		//Laplas->physic.accelerationY = 0.1;
		return 0;
	}

	//Правая прямая
	if (SegmentOverlay({ Laplas->hitbox.x - Laplas->hitbox.w / 2 ,Laplas->hitbox.y }, { Laplas->position.x - Laplas->hitbox.w / 2, Laplas->position.y },
		{ unit.x + unit.w, unit.y - Laplas->hitbox.h / 2 }, { unit.x + unit.w , unit.y + unit.h + Laplas->hitbox.h / 2 }, &intersect))
	{
		Laplas->hitbox.x = intersect.x + Laplas->hitbox.w / 2 + 1;
		Laplas->hitbox.y = Laplas->position.y;
		return 0;
	}

	//Левая прямая
	if (SegmentOverlay({ Laplas->hitbox.x + Laplas->hitbox.w /2 ,Laplas->hitbox.y  }, { Laplas->position.x + Laplas->hitbox.w /2, Laplas->position.y  },
		{ unit.x, unit.y - Laplas->hitbox.h/2}, { unit.x , unit.y + unit.h + Laplas->hitbox.h / 2 }, &intersect))
	{
		Laplas->hitbox.x = intersect.x - Laplas->hitbox.w/2 -1;
		Laplas->hitbox.y = Laplas->position.y;
		return 0;
	}

	//Нижняя прямая
	if (SegmentOverlay({ Laplas->hitbox.x ,Laplas->hitbox.y - Laplas->hitbox.h / 2 }, { Laplas->position.x, Laplas->position.y - Laplas->hitbox.h / 2 },
		{ unit.x - Laplas->hitbox.w / 2, unit.y + unit.h }, { unit.x + unit.w + Laplas->hitbox.w / 2, unit.y + unit.h }, &intersect))
	{
		Laplas->hitbox.x = Laplas->position.x;
		Laplas->hitbox.y = intersect.y + Laplas->hitbox.h / 2 + 1;
		Laplas->physic.impulse = 0.1;
		Laplas->physic.accelerationY = 0.3;
		return 0;
	}

	return 1;
}


int main(int argc, char* argv[])
{
	int bordersCount;
	FILE* f;
	SDL_Rect* bordersHitbox;
	int check = 1;
	int temp = 0;
	int time = 0;

	mainHero Laplas = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, HERO_WIDHT, HERO_HEIGHT },
				  {X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER},
				  {DASH_CD, CAMERA_SCALE_X, CAMERA_SCALE_Y} };
	
	fopen_s(&f, "Borders.txt", "r");

	fscanf_s(f, "%d", &bordersCount);
	fscanf_s(f, "%d", &Laplas.hitbox.x);
	fscanf_s(f, "%d", &Laplas.hitbox.y);

	bordersHitbox = (SDL_Rect*)malloc(sizeof(SDL_Rect) * bordersCount);

	for (int i = 0;i < bordersCount;i++)
	{
		fscanf_s(f, "%d", &bordersHitbox[i].x);
		fscanf_s(f, "%d", &bordersHitbox[i].y);
		fscanf_s(f, "%d", &bordersHitbox[i].w);
		fscanf_s(f, "%d", &bordersHitbox[i].h);
	}


	fclose(f);


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
		Laplas.position.x = Laplas.hitbox.x;
		Laplas.position.y = Laplas.hitbox.y;
		Laplas.physic.gazeDirection = Laplas.physic.xMoveL + Laplas.physic.xMoveR;

		//Движение по оси X + рывок
		Laplas.position.x +=  (Laplas.physic.xMoveL + Laplas.physic.xMoveR) * Laplas.physic.speed * Laplas.physic.accelerationX;
		if (Laplas.physic.accelerationX > 5)
			Laplas.physic.accelerationX--;
		else
			Laplas.physic.accelerationX = 1;

		//Прыжок
		if (Laplas.physic.impulse > 0.1)
		{
			Laplas.position.y -= 30 * Laplas.physic.impulse;
			if (Laplas.physic.impulse > 0.1)
				Laplas.physic.impulse -= 0.02;
			else
				Laplas.physic.accelerationY = 0.1;
			Laplas.physic.onBorder = 0;
		}

		//Гравитация
		Laplas.position.y += Laplas.physic.gravity * Laplas.physic.accelerationY;
		if (Laplas.physic.accelerationY < 1)
			Laplas.physic.accelerationY += 0.05;

		//Проверка на наложение хитбоксов
		check = 1;
		for (int i = 0;i < bordersCount;i++)
		{
			if (!CheckBorders(&Laplas, bordersHitbox[i]))
			{
				check = 0;
				break;
			}
		}
		if (check == 1)
		{
			Laplas.hitbox.x = Laplas.position.x;
			Laplas.hitbox.y = Laplas.position.y;
			Laplas.physic.onBorder = 0;
		}

		//Выход за границы мира
		if (Laplas.hitbox.x > bordersHitbox[1].w - Laplas.hitbox.w / 2 - 2)
			Laplas.hitbox.x = bordersHitbox[1].w - Laplas.hitbox.w / 2 - 2;
		else if (Laplas.hitbox.x < bordersHitbox[0].x + Laplas.hitbox.w / 2 + 2)
			Laplas.hitbox.x = bordersHitbox[0].x + Laplas.hitbox.w / 2 + 2;

		if (Laplas.hitbox.y > bordersHitbox[0].h - Laplas.hitbox.h / 2 - 2)
			Laplas.hitbox.y = bordersHitbox[0].h - Laplas.hitbox.h / 2 - 2;
		else if (Laplas.hitbox.y < bordersHitbox[0].y + Laplas.hitbox.h / 2 + 2)
			Laplas.hitbox.y = bordersHitbox[0].y + Laplas.hitbox.h / 2 + 2;

		

		#pragma endregion 

		#pragma region DRAW
		SDL_SetRenderDrawColor(ren, 255, 0, 128, 255);
		SDL_Rect movedLaplas = {Laplas.hitbox.x - Laplas.hitbox.w/2,Laplas.hitbox.y - Laplas.hitbox.h / 2, Laplas.hitbox.w, Laplas.hitbox.h };
		SDL_RenderFillRect(ren, &movedLaplas);

		static SDL_Rect rect111;
		SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
		for (int i = 0;i < bordersCount;i++)
		{
			rect111 = bordersHitbox[i];
			rect111.x -= Laplas.effect.camersScale.x;
			rect111.y -= Laplas.effect.camersScale.y;
			SDL_RenderFillRect(ren, &rect111);
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

	free(bordersHitbox);

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