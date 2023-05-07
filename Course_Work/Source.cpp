#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "func.h"
#include "common_parameters.h"
#include <math.h>
#include <time.h>
#include "HeroPhysic.h"
#include "EnemyPhysic.h"
#include "HeroBattle.h"

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

	if (Laplas->physic.xMoveL + Laplas->physic.xMoveR == 0 && !Laplas->battle.commonAtack && !Laplas->battle.shootAtack)
		Laplas->animationType = 0;
	else if (!Laplas->battle.commonAtack && !Laplas->battle.shootAtack)
		Laplas->animationType = 1;

	switch (Laplas->animationType)
	{
	case 0:
		if (Laplas->physic.gazeDirection < 0)
			SDL_RenderCopyEx(ren, Laplas->animation.com.texture, &Laplas->animation.com.frame, &movedLaplas, 0, 0, SDL_FLIP_HORIZONTAL);
		else if (Laplas->physic.gazeDirection > 0)
			SDL_RenderCopyEx(ren, Laplas->animation.com.texture, &Laplas->animation.com.frame, &movedLaplas, 0, 0, SDL_FLIP_NONE);
		if (SDL_GetTicks() - Laplas->animation.com.frameTime > 1000 / Laplas->animation.com.frameCount) 
		{
			Laplas->animation.com.frame.x += Laplas->animation.com.textureSize.w / Laplas->animation.com.frameCount;
			Laplas->animation.com.frameTime = SDL_GetTicks();
		}

		if (Laplas->animation.com.frame.x >= Laplas->animation.com.textureSize.w)
			Laplas->animation.com.frame.x = 0;
		break;

	case 1:
		if (Laplas->physic.gazeDirection < 0)
			SDL_RenderCopyEx(ren, Laplas->animation.run.texture, &Laplas->animation.run.frame, &movedLaplas, 0, 0, SDL_FLIP_HORIZONTAL);
		else if (Laplas->physic.gazeDirection > 0)
			SDL_RenderCopyEx(ren, Laplas->animation.run.texture, &Laplas->animation.run.frame, &movedLaplas, 0, 0, SDL_FLIP_NONE);
		if (SDL_GetTicks() - Laplas->animation.run.frameTime > 62 * HERO_SPEED / Laplas->animation.run.frameCount)
		{
			Laplas->animation.run.frame.x += Laplas->animation.run.textureSize.w / Laplas->animation.run.frameCount;
			Laplas->animation.run.frameTime = SDL_GetTicks();
		}

		if (Laplas->animation.run.frame.x >= Laplas->animation.run.textureSize.w)
			Laplas->animation.run.frame.x = 0;
		break;

	case 2:
		if (Laplas->physic.gazeDirection < 0)
			SDL_RenderCopyEx(ren, Laplas->animation.punch.texture, &Laplas->animation.punch.frame, &movedLaplas, 0, 0, SDL_FLIP_HORIZONTAL);
		else if (Laplas->physic.gazeDirection > 0)
			SDL_RenderCopyEx(ren, Laplas->animation.punch.texture, &Laplas->animation.punch.frame, &movedLaplas, 0, 0, SDL_FLIP_NONE);
		if (SDL_GetTicks() - Laplas->animation.punch.frameTime > HERO_ATACK_CD / Laplas->animation.punch.frameCount)
		{
			Laplas->animation.punch.frame.x += Laplas->animation.punch.textureSize.w / Laplas->animation.punch.frameCount;
			Laplas->animation.punch.frameTime = SDL_GetTicks();
		}

		if (Laplas->animation.punch.frame.x >= Laplas->animation.punch.textureSize.w )
			Laplas->animation.punch.frame.x = 0;
		break;

	case 3:
		if (Laplas->physic.gazeDirection < 0)
			SDL_RenderCopyEx(ren, Laplas->animation.shoot.texture, &Laplas->animation.shoot.frame, &movedLaplas, 0, 0, SDL_FLIP_HORIZONTAL);
		else if (Laplas->physic.gazeDirection > 0)
			SDL_RenderCopyEx(ren, Laplas->animation.shoot.texture, &Laplas->animation.shoot.frame, &movedLaplas, 0, 0, SDL_FLIP_NONE);
		if (SDL_GetTicks() - Laplas->animation.shoot.frameTime > HERO_SHOOT_CD / Laplas->animation.shoot.frameCount)
		{
			Laplas->animation.shoot.frame.x += Laplas->animation.shoot.textureSize.w / Laplas->animation.shoot.frameCount;
			Laplas->animation.shoot.frameTime = SDL_GetTicks();
		}

		if (Laplas->animation.shoot.frame.x >= Laplas->animation.shoot.textureSize.w)
			Laplas->animation.shoot.frame.x = 0;
		break;
	}


}

void DrawHitbox(int bordersCount, mainBorders levelBorders[], mainHero Laplas, mainWindow window, mainRenderer cobbleStone, mainRenderer platform)
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
			SDL_RenderCopy(ren, platform.texture, NULL, &rect123);
			break;

		case 5:
			SDL_SetRenderDrawColor(ren, 120, 0, 120, 255);
			SDL_RenderFillRect(ren, &rect123);
			break;
		}
	}
}

void DrawEnemys(int* enemysCount, mainEnemys levelEnemys[], mainHero* Laplas, mainWindow window)
{
	SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
	for (int i = 0;i < *enemysCount;i++)
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

		if (Laplas->hitbox.x >= window.w / 2.f && Laplas->hitbox.x <= levelWidth - window.w / 2.f)
			movedEnemy.x -= Laplas->hitbox.x - window.w / 2.f;
		if (Laplas->hitbox.x > levelWidth - window.w / 2.f)
			movedEnemy.x -= levelWidth - window.w;

		if (Laplas->hitbox.y >= window.h / 2.f && Laplas->hitbox.y <= levelHeight - window.h / 2.f)
			movedEnemy.y -= Laplas->hitbox.y - window.h / 2.f;
		if (Laplas->hitbox.y > levelHeight - window.h / 2.f)
			movedEnemy.y -= levelHeight - window.h;
		
		//SDL_RenderFillRect(ren, &movedEnemy);

		if (levelEnemys[i].physic.gazeDirection > 0)
			SDL_RenderCopyEx(ren, levelEnemys[i].render.texture, &levelEnemys[i].render.frame, &movedEnemy, 0, 0, SDL_FLIP_HORIZONTAL);
		else if (levelEnemys[i].physic.gazeDirection < 0)
			SDL_RenderCopyEx(ren, levelEnemys[i].render.texture, &levelEnemys[i].render.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);


		if ((levelEnemys[i].physic.xMoveL || levelEnemys[i].physic.xMoveR) && (SDL_GetTicks() - levelEnemys[i].render.frameTime > 1000 / 30) && (levelEnemys[i].physic.xMoveL + levelEnemys[i].physic.xMoveR != 0))
		{
			levelEnemys[i].render.frame.x += levelEnemys[i].render.textureSize.w / 6;
			levelEnemys[i].render.frameTime = SDL_GetTicks();
		}

		if (levelEnemys[i].render.frame.x >= levelEnemys[i].render.textureSize.w || (!levelEnemys[i].physic.xMoveL && !levelEnemys[i].physic.xMoveR) || (levelEnemys[i].physic.xMoveL + levelEnemys[i].physic.xMoveR == 0))
			levelEnemys[i].render.frame.x = 0;

		
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
	Laplas.position = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
	Laplas.hitbox = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, HERO_WIDHT, HERO_HEIGHT };
	Laplas.physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, HERO_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER, PRESSED_S };
	Laplas.effect = { HERO_DASH_CD, NULL, HERO_ATACK_CD, NULL, HERO_SHOOT_CD, NULL, CAMERA_SCALE_X, CAMERA_SCALE_Y };
	Laplas.animation.com = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas.animation.run = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas.animation.punch = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas.battle = { NULL, { NULL, NULL } , NULL, NULL};
	Laplas.status = { HERO_DAMAGE, HERO_HP, ALIVE , HERO_START_AMUNITION, HERO_SHOOT_DAMAGE};
	Laplas.animationType = NULL;

	return Laplas;
}

void InitEnemys(mainEnemys levelEnemys[], int enemysCount, mainRenderer* texture_beaver)
{
	for (int i = 0; i < enemysCount;i++)
	{
		if (levelEnemys[i].type == 1)
		{
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, BEAVER_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status = { BEAVER_DMG, BEAVER_HP , ALIVE };
			levelEnemys[i].effect.atackCD = BEAVER_ATACK_CD;
			levelEnemys[i].render = *texture_beaver;
		}

		else if (levelEnemys[i].type == 2)
		{
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, BEAVER_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status = { BEAVER_DMG, BEAVER_HP , ALIVE };
			levelEnemys[i].effect.atackCD = BEAVER_ATACK_CD;
			levelEnemys[i].render = *texture_beaver;
		}
	}
}

mainRenderer CreateTimerFromText(const char str[], TTF_Font* font, SDL_Color fg)
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, str, fg);
	mainRenderer texture;
	texture.texture = SDL_CreateTextureFromSurface(ren, surface);
	texture.textureSize.w = surface->w;
	texture.textureSize.h = surface->h;
	texture.textureSize.x = WINDOW_WIDTH - surface->w;
	texture.textureSize.y = NULL;
	SDL_FreeSurface(surface);
	return texture;
}

mainRenderer CreateAmmunitionFromText(const char str[], TTF_Font* font, SDL_Color fg)
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

void GetTexture(const char filePath[], mainRenderer* texture, int frameCount)
{
	SDL_Surface* surface = NULL;
	if ((surface = IMG_Load(filePath)) == NULL)
	{
		printf_s("Can't load image '%s'", filePath);
		system("pause");
	}

	texture->texture = SDL_CreateTextureFromSurface(ren, surface);

	texture->textureSize.x = NULL;
	texture->textureSize.y = NULL;
	texture->textureSize.w = surface->w;
	texture->textureSize.h = surface->h;

	texture->frame.x = NULL;
	texture->frame.y = NULL;
	texture->frame.w = surface->w / frameCount;
	texture->frame.h = surface->h;

	texture->frameCount = frameCount;

	SDL_FreeSurface(surface);
}

void MainMenu(GameState* gameState) {
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	SDL_Rect startButton = { 560,240,160,50 };
	SDL_Rect settingsButton = { 560,340,160,50 };
	SDL_Rect creditsButton = { 560,440,160,50 };
	SDL_Rect exitButton = { 560,540,160,50 };

	SDL_Rect buttons[4] = { startButton, settingsButton, creditsButton, exitButton };
	SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
	for (int i = 0; i < 4; i++)
	{
		SDL_RenderDrawRect(ren, &buttons[i]);
	}
	SDL_RenderPresent(ren);

	while (true)
	{
		while (SDL_PollEvent(&ev)) 
		{
			switch (ev.button.button)
			{
				case SDL_BUTTON_LEFT:
				{
					SDL_Point mousePoint = { ev.button.x,ev.button.y };
					for (int i = 0; i < 4; i++)
					{
						if (SDL_PointInRect(&mousePoint, &buttons[i])) 
						{
							*gameState = (GameState)i;
							return;
						}
					}
				}
			}
		}
	}
}

void PauseMenu(GameState* gameState) {
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	SDL_Rect continueButton = { 560,240,160,50 };
	SDL_Rect exitButton = { 560,340,160,50 };

	SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
	SDL_RenderDrawRect(ren, &continueButton);
	SDL_RenderDrawRect(ren, &exitButton);

	SDL_RenderPresent(ren);


	while (true)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				SDL_Point mousePoint = { ev.button.x,ev.button.y };
				if (SDL_PointInRect(&mousePoint, &continueButton))
				{
					SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
					SDL_RenderClear(ren);
					*gameState = IN_GAME;
					return;
				}
				else if (SDL_PointInRect(&mousePoint, &exitButton))
				{
					SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
					SDL_RenderClear(ren);
					*gameState = MAIN_MENU;
					return;
				}
			}
			}
		}
	}
}

void SettingsMenu(GameState* gameState, Settings*  settings) {

	SDL_Rect volumeLeftButton = { 410,240,50,50 };
	SDL_Rect volumeBar;
	SDL_Rect volumeRightButton = { 770,240,50,50 };
	

	SDL_Rect skinLeftButton = { 410,440,50,50 };
	SDL_Rect skinRightButton = { 770,440,50,50 };
	

	SDL_Rect exitButton = { 540,640,160,50 };

	while (true)
	{
		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
		SDL_RenderDrawRect(ren, &volumeLeftButton);
		SDL_RenderDrawRect(ren, &volumeRightButton);

		volumeBar = { 490,240,50 * settings->volume,50 };
		SDL_RenderDrawRect(ren, &volumeBar);

		SDL_RenderDrawRect(ren, &skinLeftButton);
		SDL_RenderDrawRect(ren, &skinRightButton);

		SDL_RenderDrawRect(ren, &exitButton);

		SDL_RenderPresent(ren);

		while (SDL_PollEvent(&ev))
		{
			switch (ev.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				SDL_Point mousePoint = { ev.button.x,ev.button.y };
				if (SDL_PointInRect(&mousePoint, &volumeLeftButton) && settings->volume > 0) {
					settings->volume--;
				}
				else if (SDL_PointInRect(&mousePoint, &volumeRightButton) && settings->volume < 5) {
					settings->volume++;
				}

				if (SDL_PointInRect(&mousePoint, &skinLeftButton) && settings->skin > 0) {
					settings->skin--;
				}
				else if (SDL_PointInRect(&mousePoint, &skinRightButton) && settings->skin < 5) {
					settings->skin++;
				}

				if (SDL_PointInRect(&mousePoint, &exitButton))
				{
					SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
					SDL_RenderClear(ren);
					*gameState = MAIN_MENU;
					return;
				}
			}
			}
		}

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
	}
}

void CreditsMenu(GameState* gameState) {
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	SDL_Rect exitButton = { 540,640,160,50 };
	SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
	SDL_RenderDrawRect(ren, &exitButton);
	SDL_RenderPresent(ren);

	while (true)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				SDL_Point mousePoint = { ev.button.x,ev.button.y };

				if (SDL_PointInRect(&mousePoint, &exitButton))
				{
					SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
					SDL_RenderClear(ren);
					*gameState = MAIN_MENU;
					return;
				}
			}
			}
		}
	}
}


void DrawBullet()
{

}

int main(int argc, char* argv[])
{
	bool flag = 1;
	Init(&win, &ren, &win_surface);

	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	#pragma region VARIABLE_INIT

	GameState gameState = MAIN_MENU;
	Settings settings = { 5, 1 };

	mainRenderer texture_backGround;
	mainRenderer texture_cobbleStone;
	mainRenderer texture_hpBar;
	mainRenderer texture_timer;
	mainRenderer texture_ammunition;
	mainRenderer texture_beaver;
	mainRenderer texture_platform;
	TTF_Font* fontNovemBig = NULL;
	TTF_Font* fontNovemSmall = NULL;

	int bordersCount;
	int enemysCount = NULL;
	int lastTime = 0;
	int temp = 0;
	int deltaTime = clock();
	int check = 1;

	mainBorders* levelBorders = NULL;
	mainEnemys* levelEnemys = NULL;
	mainHero Laplas;
	
	static mainWindow window = { WINDOW_WIDTH ,WINDOW_HEIGHT };
	SDL_Point mouseClick = { NULL, NULL };
	
	char timer_text[10] = "00:00";
	char amunition_text[10] = "000";
	
	bool isRunning;

	#pragma endregion

	Laplas = InitHero();

	#pragma region TEXTURES_LOAD

	GetTexture("Textures\\hero_comm.png", &Laplas.animation.com, 3);
	GetTexture("Textures\\hero_run.png", &Laplas.animation.run, 8);
	GetTexture("Textures\\hero_atack.png", &Laplas.animation.punch, 5);
	GetTexture("Textures\\hero_shoot.png", &Laplas.animation.shoot, 3);
	Laplas.hitbox.h *= (Laplas.animation.com.frame.w / 1. / Laplas.animation.com.frame.h);

	GetTexture("Textures\\bobr.png", &texture_beaver, 6);

	GetTexture("Textures\\BackGroundCave.png", &texture_backGround, 1);

	GetTexture("Textures\\cobblestone40x40.png", &texture_cobbleStone, 1);

	GetTexture("Textures\\woodenPlatform.png", &texture_platform, 1);

	#pragma endregion

	#pragma region TIMER_TEXTURE

	SDL_Surface* surface = NULL;
	if ((fontNovemBig = TTF_OpenFont("Fonts\\novem.ttf", TIMER_TEXT_SIZE)) == NULL)
	{
		printf_s("Can't open 'novem.ttf'\n");
		system("pause");
	}

	surface = TTF_RenderText_Blended(fontNovemBig, timer_text, { 100, 100, 100, 255 });

	texture_timer.texture = SDL_CreateTextureFromSurface(ren, surface);
	texture_timer.textureSize.w = surface->w;
	texture_timer.textureSize.h = surface->h;
	texture_timer.textureSize.x = WINDOW_WIDTH - surface->w;
	texture_timer.textureSize.y = NULL;

	SDL_FreeSurface(surface);

	#pragma endregion

	#pragma region AMMUNITION_TEXTURE

	if ((fontNovemSmall = TTF_OpenFont("Fonts\\novem.ttf", AMMUNITION_TEXT_SIZE)) == NULL)
	{
		printf_s("Can't open 'novem.ttf'\n");
		system("pause");
	}

	surface = NULL;

	surface = TTF_RenderText_Blended(fontNovemSmall, timer_text, { 255, 215, 0, 255 });

	texture_ammunition.texture = SDL_CreateTextureFromSurface(ren, surface);
	texture_ammunition.textureSize.w = surface->w;
	texture_ammunition.textureSize.h = surface->h;
	texture_ammunition.textureSize.x = NULL;
	texture_ammunition.textureSize.y = NULL;

	SDL_FreeSurface(surface);

	#pragma endregion

	levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Levels/Borders.txt");
	levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemys/Enemy.txt");

	InitEnemys(levelEnemys, enemysCount, &texture_beaver);



	while (flag)
	{
		switch (gameState)
		{
		case MAIN_MENU:
			MainMenu(&gameState);
			break;

		case IN_GAME:
			isRunning = true;
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
							gameState = PAUSE_MENU;
							break;

						case SDL_SCANCODE_A:
							Laplas.physic.xMoveL = -1;
							break;

						case SDL_SCANCODE_D:
							Laplas.physic.xMoveR = 1;
							break;

						case SDL_SCANCODE_S:
							Laplas.physic.pressed_S = 1;
							break;

						case SDL_SCANCODE_LSHIFT:
							if (deltaTime - Laplas.effect.timeDashCD > Laplas.effect.dashCD)
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

						case SDL_SCANCODE_S:
							Laplas.physic.pressed_S = 0;
							break;

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
							if (!Laplas.battle.commonAtack && !Laplas.battle.shootAtack)
							{
								Laplas.effect.timeAtackCD = deltaTime;
								Laplas.battle.commonAtack = 1;
							}
						}
						else if (ev.button.button == SDL_BUTTON_RIGHT)
						{
							if (Laplas.status.ammunition > 0 && !Laplas.battle.shootAtack && !Laplas.battle.commonAtack)
							{
								Laplas.effect.timeShootCD = deltaTime;
								Laplas.battle.shootAtack = 1;
								Laplas.status.ammunition -= 1;
								AddNewBullet(&Laplas);
							}
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
				if(Laplas.animationType != 2 && Laplas.animationType != 3)
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

				//Движение врагов
				for (int i = 0; i < enemysCount; i++)
				{
					if (levelEnemys[i].status.alive)
					{
						if (levelEnemys[i].hitbox.x < Laplas.hitbox.x)
						{
							levelEnemys[i].hitbox.x += levelEnemys[i].physic.speed;
							levelEnemys[i].physic.xMoveR = 1;
							levelEnemys[i].physic.xMoveL = 0;
						}
						else if (levelEnemys[i].hitbox.x > Laplas.hitbox.x)
						{
							levelEnemys[i].hitbox.x -= levelEnemys[i].physic.speed;
							levelEnemys[i].physic.xMoveR = 0;
							levelEnemys[i].physic.xMoveL = -1;
						}
					}
				}

				//Переход на другую локацию
				if (HeroPhysicInRange({ Laplas.hitbox.x, Laplas.hitbox.y }, levelBorders[9].bordersHitbox))
				{
					levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Levels/Borders1.txt");
					levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemys/Enemy1.txt");
					InitEnemys(levelEnemys, enemysCount, &texture_beaver);
				}

				#pragma endregion 

				#pragma region BATTLE

				HeroCommonAtack(&Laplas, &deltaTime, &enemysCount, levelEnemys);

				HeroShootAtack(&Laplas, &deltaTime, &enemysCount, levelEnemys);

				#pragma endregion 

				#pragma region DRAW
				SDL_RenderCopy(ren, texture_backGround.texture, NULL, NULL);
		
				DrawHitbox(bordersCount, levelBorders, Laplas, window, texture_cobbleStone, texture_platform);
				DrawEnemys(&enemysCount, levelEnemys, &Laplas, window);
		
				//Отрисовка таймера
				if (lastTime != deltaTime / 1000 % 60)
				{
					lastTime = deltaTime / 1000 % 60;
					sprintf_s(timer_text, "%02i:%02i", deltaTime / 60000, lastTime);
					SDL_DestroyTexture(texture_timer.texture);
					texture_timer = CreateTimerFromText(timer_text, fontNovemBig, { 100, 100, 100, 255 });
				}

				//Отрисовка кол-ва боеприпасов
				sprintf_s(amunition_text, "%03i", Laplas.status.ammunition);
				SDL_DestroyTexture(texture_ammunition.texture);
				texture_ammunition = CreateAmmunitionFromText(amunition_text, fontNovemSmall, { 255, 215, 0, 255 });

		
				SDL_RenderCopy(ren, texture_timer.texture, NULL, &texture_timer.textureSize);
				SDL_RenderCopy(ren, texture_ammunition.texture, NULL, &texture_ammunition.textureSize);
		
				for(int k = 0; k < 10;k++)
					if (Laplas.battle.shoot[k].alive)
					{
						SDL_Rect tmmmm = { Laplas.battle.shoot[k].shootAtackCentere.x - 2, Laplas.battle.shoot[k].shootAtackCentere.y - 2, 4, 4};
						SDL_SetRenderDrawColor(ren, 255, 215, 0, 255);
						SDL_RenderFillRect(ren, &tmmmm);
					}

				//DrawBullets();

				DrawMainHero(&Laplas, window);
				SDL_RenderPresent(ren);

				SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
				SDL_RenderClear(ren);
				#pragma endregion 

				FPSControl();
			}
			break;

		case CREDITS:
			CreditsMenu(&gameState);
			break;

		case SETTINGS:
			SettingsMenu(&gameState,&settings);
			break;

		case PAUSE_MENU:
			PauseMenu(&gameState);
			break;

		case QUIT:
			flag = 0;
			break;
		}
		

	}



	

	free(levelBorders);
	free(levelEnemys);
	TTF_CloseFont(fontNovemBig);
	TTF_CloseFont(fontNovemSmall);

	SDL_DestroyTexture(texture_backGround.texture);
	SDL_DestroyTexture(texture_cobbleStone.texture);
	SDL_DestroyTexture(texture_timer.texture);
	SDL_DestroyTexture(texture_beaver.texture);
	SDL_DestroyTexture(Laplas.animation.com.texture);
	SDL_DestroyTexture(texture_platform.texture);
	

	DeInit(0, &win, &ren, &win_surface);
	
	return 0;
}
