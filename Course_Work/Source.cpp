#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
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


void DrawMainHero(mainHero* Laplas, mainWindow window)
{
	SDL_SetRenderDrawColor(ren, 255, 0, 128, 255);
	SDL_Rect movedLaplas = { window.w/2.f - Laplas->hitbox.w / 2.f,window.h / 2.f - Laplas->hitbox.h / 2.f, Laplas->hitbox.w, Laplas->hitbox.h };

	if (Laplas->hitbox.x <= window.w / 2.f)
		movedLaplas.x = Laplas->hitbox.x - Laplas->hitbox.w / 2.f;
	if(Laplas->hitbox.x > levelWidth - window.w / 2.f)
		movedLaplas.x = Laplas->hitbox.x - Laplas->hitbox.w / 2.f - (levelWidth - window.w);

	if (Laplas->hitbox.y <= window.h / 2.f)
		movedLaplas.y = Laplas->hitbox.y - Laplas->hitbox.h / 2.f;
	if (Laplas->hitbox.y > levelHeight - window.h / 2.f)
		movedLaplas.y = Laplas->hitbox.y - Laplas->hitbox.h / 2.f - (levelHeight - window.h);
	SDL_Point p = { 100, -100 };
	//SDL_RenderFillRect(ren, &movedLaplas);
	if(Laplas->physic.gazeDirection > 0)
		SDL_RenderCopyEx(ren, Laplas->render.texture, &Laplas->render.frame, &movedLaplas, 0, 0, SDL_FLIP_HORIZONTAL);
	else if(Laplas->physic.gazeDirection < 0)
		SDL_RenderCopyEx(ren, Laplas->render.texture, &Laplas->render.frame, &movedLaplas, 0, 0, SDL_FLIP_NONE);


	if ((Laplas->physic.xMoveL || Laplas->physic.xMoveR) && (SDL_GetTicks() - Laplas->render.frameTime > 1000/30) && (Laplas->physic.xMoveL + Laplas->physic.xMoveR != 0))
	{
		Laplas->render.frame.x += Laplas->render.textureSize.w / 6;
		Laplas->render.frameTime = SDL_GetTicks();
	}

	if (Laplas->render.frame.x >= Laplas->render.textureSize.w || (!Laplas->physic.xMoveL && !Laplas->physic.xMoveR) || (Laplas->physic.xMoveL + Laplas->physic.xMoveR == 0))
		Laplas->render.frame.x = 0;
}

void DrawHitbox(int bordersCount, mainBorders levelBorders[], mainHero Laplas, mainWindow window, mainRenderer cobbleStone)
{
	SDL_Rect rect123;
	SDL_Rect dopRect = {NULL, NULL, NULL, NULL};
	bool check = 1;
	SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
	for (int i = 0;i < bordersCount;i++)
	{
		rect123 = levelBorders[i].bordersHitbox;

		if (Laplas.hitbox.x >= window.w / 2.f && Laplas.hitbox.x <= levelWidth - window.w / 2.f)
			rect123.x -= Laplas.hitbox.x - window.w / 2.f;
		if (Laplas.hitbox.x > levelWidth - window.w / 2.f)
			rect123.x -= levelWidth - window.w;

		if (Laplas.hitbox.y >= window.h / 2.f && Laplas.hitbox.y <= levelHeight - window.h / 2.f)
			rect123.y -= Laplas.hitbox.y - window.h / 2.f;
		if (Laplas.hitbox.y > levelHeight - window.h / 2.f)
			rect123.y -= levelHeight - window.h;

		switch (levelBorders[i].type)
		{
		case 1:
			SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
			SDL_RenderFillRect(ren, &rect123);
			break;

		case 2:
			SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
			SDL_RenderFillRect(ren, &rect123);
			SDL_Rect rect1 = cobbleStone.textureSize;
			dopRect = cobbleStone.textureSize;

			for (int i = rect123.y; i < rect123.y + rect123.h; i+= rect1.h)
				for (int j = rect123.x; j < rect123.x + rect123.w; j+= rect1.w)
				{
					if (j + rect1.w > rect123.x + rect123.w)
					{
						rect1.w = rect123.x + rect123.w - j;
						dopRect.w = rect1.w;
						check = 1;
					}
					if (j + rect1.h > rect123.y + rect123.h)
					{
						rect1.h = rect123.y + rect123.h - i;
						dopRect.h = rect1.h;
						check = 1;
					}
					rect1.x = j;
					rect1.y = i;
					SDL_RenderCopy(ren, cobbleStone.texture, &dopRect, &rect1);
					if (check)
					{
						rect1.h = cobbleStone.textureSize.h;
						rect1.w = cobbleStone.textureSize.w;
						dopRect.h = cobbleStone.textureSize.h;
						dopRect.w = cobbleStone.textureSize.w;
						check = 0;
					}

				}
				break;

		case 3:
			SDL_SetRenderDrawColor(ren, 255, 128, 128, 255);
			SDL_RenderFillRect(ren, &rect123);
			break;

		case 4:
			SDL_SetRenderDrawColor(ren, 200, 200, 0, 255);
			SDL_RenderFillRect(ren, &rect123);
			break;
		}
	}
}

void DrawEnemys(int enemysCount, mainEnemys levelEnemys[], mainHero Laplas, mainWindow window)
{
	SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
	for (int i = 0;i < enemysCount;i++)
	{
		switch (levelEnemys[i].type)
		{
		case 1:
			SDL_SetRenderDrawColor(ren, 129, 0, 255, 255);
			break;
		case 2:
			SDL_SetRenderDrawColor(ren, 0, 129, 255, 255);
			break;
		}

		SDL_Rect movedEnemy = { levelEnemys[i].hitbox.x - levelEnemys[i].hitbox.w / 2,levelEnemys[i].hitbox.y - levelEnemys[i].hitbox.h / 2, levelEnemys[i].hitbox.w, levelEnemys[i].hitbox.h};
		

		//movedEnemy = levelEnemys[i].hitbox;

		if (Laplas.hitbox.x >= window.w / 2.f && Laplas.hitbox.x <= levelWidth - window.w / 2.f)
			movedEnemy.x -= Laplas.hitbox.x - window.w / 2.f;
		if (Laplas.hitbox.x > levelWidth - window.w / 2.f)
			movedEnemy.x -= levelWidth - window.w;

		if (Laplas.hitbox.y >= window.h / 2.f && Laplas.hitbox.y <= levelHeight - window.h / 2.f)
			movedEnemy.y -= Laplas.hitbox.y - window.h / 2.f;
		if (Laplas.hitbox.y > levelHeight - window.h / 2.f)
			movedEnemy.y -= levelHeight - window.h;

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
			  {DASH_CD, NULL, ATACK_CD, NULL, CAMERA_SCALE_X, CAMERA_SCALE_Y}, NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL,
			  NULL, {NULL, NULL }, {HERO_DAMAGE, HERO_HP, ALIVE} };
}

void InitEnemys(mainEnemys levelEnemys[], int enemysCount)
{
	for (int i = 0; i < enemysCount;i++)
	{
		levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
		levelEnemys[i].effect.underAtack = NULL;
		levelEnemys[i].status = { HERO_DAMAGE, HERO_HP , ALIVE};
	}
}

bool CheckAtackHitbox(SDL_Rect* hero, SDL_Rect* enemy)
{
	// Вычисляем расстояние между центрами окружностей
	float distance = sqrt((hero->x - enemy->x) * (hero->x - enemy->x) + (hero->y - enemy->y) * (hero->y - enemy->y));

	// Если расстояние меньше или равно сумме радиусов, то окружности пересекаются
	if (distance <= (hero->w + hero->h) / 4 + (enemy->w + enemy->h) / 4)
		return 1;
	else
		return 0;
}

mainRenderer CreateTextureFromText(const char str[], TTF_Font* font, SDL_Color fg)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, str, fg);
	mainRenderer texture;
	texture.texture = SDL_CreateTextureFromSurface(ren, surface);
	texture.textureSize.w = surface->w;
	texture.textureSize.h = surface->h;
	texture.textureSize.x = NULL;
	texture.textureSize.y = NULL;
	SDL_FreeSurface(surface);
	return texture;
}

//SDL_Texture* resizeTexture(SDL_Renderer* renderer, SDL_Texture* texture, int newWidth, int newHeight)
//{
//	SDL_Surface* surface = SDL_CreateRGBSurface(0, newWidth, newHeight, 32, 0, 0, 0, 0);
//	SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, newWidth, newHeight);
//	SDL_BlitScaled(texture, NULL, surface, NULL);
//	SDL_UpdateTexture(newTexture, NULL, surface->pixels, surface->pitch);
//	SDL_FreeSurface(surface);
//	SDL_DestroyTexture(texture);
//	return newTexture;
//}

int main(int argc, char* argv[])
{
	Init(&win, &ren, &win_surface);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	mainRenderer backGround;
	mainRenderer cobbleStone;
	mainRenderer hpBar;
	mainRenderer timer;
	TTF_Font* fontNovem = NULL;

	int bordersCount;
	int enemysCount;
	mainBorders* levelBorders = NULL;
	mainEnemys* levelEnemys = NULL;
	int check = 1;
	mainHero Laplas;
	int temp = 0;
	static mainWindow window = { WINDOW_WIDTH ,WINDOW_HEIGHT };
	SDL_Point mouseClick = { NULL, NULL };
	int deltaTime = clock();
	char timer_text[10] = "00:00";
	int lastTime = 0;

	Laplas = InitHero();
	
	#pragma region MAIN_HERO TEXTURE
		SDL_Surface* surface = NULL;
		if((surface = IMG_Load("Textures/bobr.png")) == NULL)
		{
			printf_s("Can't load image 'bobr.png'");
			system("pause");
		}

		Laplas.render.texture = SDL_CreateTextureFromSurface(ren, surface);
		Laplas.render.textureSize.w = surface->w;
		Laplas.render.textureSize.h = surface->h;
		Laplas.render.frame.w = surface->w / 6;
		Laplas.render.frame.h = surface->h;
		SDL_FreeSurface(surface);
	#pragma endregion

	#pragma region BACKGROUND TEXTURE
		surface = NULL;
		if ((surface = IMG_Load("Textures/BackGroundCave.png")) == NULL)
		{
			printf_s("Can't load image 'BackGroundCave.png'");
			system("pause");
		}

		backGround.texture = SDL_CreateTextureFromSurface(ren, surface);
		backGround.textureSize.w = surface->w;
		backGround.textureSize.h = surface->h;
		backGround.frame.w = surface->w;
		backGround.frame.h = surface->h;
		SDL_FreeSurface(surface);
	#pragma endregion

	#pragma region COBBLESTONE TEXTURE
	surface = NULL;
	if ((surface = IMG_Load("Textures/cobblestone40x40.png")) == NULL)
	{
		printf_s("Can't load image 'cobblestone40x40.png'");
		system("pause");
	}

	cobbleStone.texture = SDL_CreateTextureFromSurface(ren, surface);
	
	cobbleStone.textureSize.x = NULL;
	cobbleStone.textureSize.y = NULL;
	cobbleStone.textureSize.w = surface->w * 2.5;
	cobbleStone.textureSize.h = surface->h * 2.5;

	cobbleStone.frame.w = surface->w * 2.5;
	cobbleStone.frame.h = surface->h * 2.5;
	SDL_FreeSurface(surface);
	#pragma endregion

	#pragma region HP_BAR TEXTURE
	surface = NULL;
	if ((surface = IMG_Load("Textures/hpBar.png")) == NULL)
	{
		printf_s("Can't load image 'hpBar.png'");
		system("pause");
	}
			
	hpBar.texture = SDL_CreateTextureFromSurface(ren, surface);

	hpBar.textureSize.x = NULL;
	hpBar.textureSize.y = NULL;
	hpBar.textureSize.w = surface->w * 0.5;
	hpBar.textureSize.h = surface->h * 0.5;

	hpBar.frame.w = surface->w * 0.5;
	hpBar.frame.h = surface->h * 0.5;
	SDL_FreeSurface(surface);
	#pragma endregion

	#pragma region TIMER_TEXTURE

	surface = NULL;
	if ((fontNovem = TTF_OpenFont("Fonts\\novem.ttf", TIMER_SIZE)) == NULL)
	{
		printf_s("Can't open 'novem.ttf'\n");
		system("pause");
	}

	surface = TTF_RenderText_Blended(fontNovem, timer_text, { 100, 100, 100, 255 });

	timer.texture = SDL_CreateTextureFromSurface(ren, surface);
	timer.textureSize.w = surface->w;
	timer.textureSize.h = surface->h;
	timer.textureSize.x = NULL;
	timer.textureSize.y = NULL;

	SDL_FreeSurface(surface);

	#pragma endregion

	levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Levels/Borders.txt");
	levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemys/Enemy.txt");

	InitEnemys(levelEnemys, enemysCount);

	

	bool isRunning = true;

	while (isRunning)
	{
		deltaTime = clock();

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
					SDL_RenderSetScale(ren, window.scaleX = window.w / 1. / WINDOW_WIDTH, window.scaleY = window.h / 1. / WINDOW_HEIGHT);
					window.w /= window.scaleX;
					window.h /= window.scaleY;
				
					break;
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
					break;
				case SDL_SCANCODE_D:
					Laplas.physic.xMoveR = 1;
					break;
				case SDL_SCANCODE_LSHIFT:
					if (deltaTime - Laplas.effect.timeDashCD > Laplas.effect.dashCD )
					{
						Laplas.effect.timeDashCD = deltaTime;
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
					if (deltaTime - Laplas.effect.timeDashCD > Laplas.effect.dashCD)
					{
						Laplas.effect.timeDashCD = deltaTime;
						Laplas.physic.accelerationX = 8;
					}
				}
				break;


			case SDL_MOUSEBUTTONUP:
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					if (Laplas.battle.commonAtack == 0)
					{
						Laplas.effect.timeAtackCD = deltaTime;
						Laplas.battle.commonAtack = 1;
					}
				}
				else if (ev.button.button == SDL_BUTTON_RIGHT)
				{
					SDL_Rect rrr = {window.w*window.scaleX - 100, window.h*window.scaleY - 100, 100, 100};
					SDL_GetMouseState(&mouseClick.x, &mouseClick.y);
					if (SDL_PointInRect(&mouseClick, &rrr))
						printf_s("click\n");
				}
				break;
			}



		}
		#pragma endregion

		#pragma region PHYSIC_CHECK

		//Получение координат
		HeroPhysicGetBase(&Laplas);
		EnemyPhysicGetBase(levelEnemys, &enemysCount);

		//Движение по оси X + рывок
		HeroPhysicXmovement(&Laplas);

		//Прыжок
		HeroPhysicJump(&Laplas);

		//Гравитация
		HeroPhysicGravity(&Laplas);
		EnemyPhysicGravity(levelEnemys, &enemysCount);
		
		//Проверка на наложение хитбоксов
		HeroPhysicHitboxOverlay(&bordersCount, &Laplas, levelBorders);
		EnemyPhysicHitboxOverlay(&bordersCount, &enemysCount, levelEnemys, levelBorders);

		//Выход за границы мира
		HeroPhysicOutworldCheck(&Laplas, levelBorders);
		EnemyPhysicOutworldCheck(&enemysCount, levelEnemys, levelBorders);

		for (int i = 0; i < enemysCount; i++)
		{
			if (levelEnemys[i].status.alive)
			{
				if (levelEnemys[i].hitbox.x < Laplas.hitbox.x)
				{
					levelEnemys[i].hitbox.x += levelEnemys[i].physic.speed;
				}
				else if (levelEnemys[i].hitbox.x > Laplas.hitbox.x)
				{
					levelEnemys[i].hitbox.x -= levelEnemys[i].physic.speed;
				}
			}
		}

		if (HeroPhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, levelBorders[8].bordersHitbox))
		{
			levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Levels/Borders1.txt");
			levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemys/Enemy1.txt");
			InitEnemys(levelEnemys, enemysCount);
		}

		#pragma endregion 

		#pragma region BATTLE

		if (Laplas.battle.commonAtack && Laplas.effect.timeAtackCD + Laplas.effect.atackCD > deltaTime)
		{
			//анимация
		}
		else
		{
			Laplas.battle.commonAtack = 0;
			for (int i = 0;i < enemysCount; i++)
				levelEnemys[i].effect.underAtack = 0;
		}

		for (int i = 0; i < enemysCount; i++)
		{
			if (Laplas.battle.commonAtack && levelEnemys[i].status.alive && !levelEnemys[i].effect.underAtack && CheckAtackHitbox(&Laplas.hitbox, &levelEnemys[i].hitbox))
			{
				levelEnemys[i].effect.underAtack = 1;
				levelEnemys[i].status.HP -= Laplas.status.DMG;
				if (levelEnemys[i].status.HP <= 0)
				{
					levelEnemys[i].status.alive = 0;
				}
			}
		}

		


		#pragma endregion 

		#pragma region DRAW
		SDL_RenderCopy(ren, backGround.texture, NULL, NULL);
		DrawMainHero(&Laplas, window);

		DrawHitbox(bordersCount, levelBorders, Laplas, window, cobbleStone);
		DrawEnemys(enemysCount, levelEnemys, Laplas, window);
		
		if (lastTime != deltaTime / 1000 % 60)
		{
			lastTime = deltaTime / 1000 % 60;
			sprintf_s(timer_text, "%02i:%02i", deltaTime / 60000, lastTime);
			SDL_DestroyTexture(timer.texture);
			timer = CreateTextureFromText(timer_text, fontNovem, { 100, 100, 100, 255 });
		}

		
		SDL_RenderCopy(ren, timer.texture, NULL, &timer.textureSize);

		//SDL_Rect tempRect = hpBar.textureSize;
		////tempRect.h /= 2;
		//SDL_Rect tempRect2 = { 0, 0, hpBar.textureSize.w, hpBar.textureSize.h/2 };
		//
		//

		//SDL_RenderCopy(ren, hpBar.texture, &tempRect2, &tempRect);
		

		SDL_RenderPresent(ren);



		SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
		SDL_RenderClear(ren);
		#pragma endregion 

		#pragma region FPS_DELAY

		FPSControl();

		#pragma endregion 
	}

	free(levelBorders);
	free(levelEnemys);
	TTF_CloseFont(fontNovem);

	SDL_DestroyTexture(backGround.texture);
	SDL_DestroyTexture(cobbleStone.texture);
	SDL_DestroyTexture(hpBar.texture);
	SDL_DestroyTexture(timer.texture);
	SDL_DestroyTexture(Laplas.render.texture);


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