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

void DrawHitbox(int bordersCount, mainBorders levelBorders[], mainHero* Laplas, mainWindow* window, mainRenderer* cobbleStone, mainRenderer* platform, mainRenderer* dart_trap)
{
	SDL_Rect rect123;
	SDL_Rect dopRect = {NULL, NULL, NULL, NULL};
	bool check = 1;
	SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
	for (int i = 0;i < bordersCount;i++)
	{
		rect123 = levelBorders[i].bordersHitbox;

		if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
			rect123.x -= Laplas->hitbox.x - window->w / 2.f;
		if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
			rect123.x -= levelWidth - window->w;

		if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
			rect123.y -= Laplas->hitbox.y - window->h / 2.f;
		if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
			rect123.y -= levelHeight - window->h;

		switch (levelBorders[i].type)
		{
		case 1:
			SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
			SDL_RenderFillRect(ren, &rect123);
			break;

		case 2:
			SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
			SDL_RenderFillRect(ren, &rect123);
			SDL_Rect rect1 = cobbleStone->textureSize;
			dopRect = cobbleStone->textureSize;

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
					SDL_RenderCopy(ren, cobbleStone->texture, &dopRect, &rect1);
					if (check)
					{
						rect1.h = cobbleStone->textureSize.h;
						rect1.w = cobbleStone->textureSize.w;
						dopRect.h = cobbleStone->textureSize.h;
						dopRect.w = cobbleStone->textureSize.w;
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
			SDL_RenderCopy(ren, platform->texture, NULL, &rect123);
			break;

		case 5:
			SDL_SetRenderDrawColor(ren, 120, 0, 120, 255);
			SDL_RenderFillRect(ren, &rect123);
			break;

		case 6:
			SDL_RenderCopy(ren, dart_trap->texture, NULL, &rect123);
			break;
		}
	}
}

void DrawEnemys(int* enemysCount, mainEnemys levelEnemys[], mainHero* Laplas, mainWindow* window)
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

		if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
			movedEnemy.x -= Laplas->hitbox.x - window->w / 2.f;
		if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
			movedEnemy.x -= levelWidth - window->w;

		if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
			movedEnemy.y -= Laplas->hitbox.y - window->h / 2.f;
		if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
			movedEnemy.y -= levelHeight - window->h;
		
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

void DrawTraps(int* trapsCount, mainTraps levelTraps[], mainHero* Laplas, mainWindow* window)
{
	for (int i = 0; i < *trapsCount; i++)
	{

		SDL_Rect movedTrap = { levelTraps[i].hitbox.x,levelTraps[i].hitbox.y, levelTraps[i].hitbox.w, levelTraps[i].hitbox.h };

		if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
			movedTrap.x -= Laplas->hitbox.x - window->w / 2.f;
		if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
			movedTrap.x -= levelWidth - window->w;

		if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
			movedTrap.y -= Laplas->hitbox.y - window->h / 2.f;
		if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
			movedTrap.y -= levelHeight - window->h;

		if (levelTraps[i].gazeDirection == 1)
			SDL_RenderCopyEx(ren, levelTraps[i].render.texture, NULL, &movedTrap, 0, 0, SDL_FLIP_HORIZONTAL);
		else if (levelTraps[i].gazeDirection == 0)
			SDL_RenderCopyEx(ren, levelTraps[i].render.texture, NULL, &movedTrap, 0, 0, SDL_FLIP_NONE);

	}
}

void DrawHeroBullet(mainHero* Laplas, mainWindow* window)
{
	for (int i = 0; i < 10; i++)
		if (Laplas->battle.shoot[i].alive)
		{
			SDL_Rect rect123 = { Laplas->battle.shoot[i].shootAtackCentere.x - HERO_BULLET_WIDTH / 2 ,Laplas->battle.shoot[i].shootAtackCentere.y - HERO_BULLET_HIGHT / 2, HERO_BULLET_WIDTH, HERO_BULLET_HIGHT };

			if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
				rect123.x -= Laplas->hitbox.x - window->w / 2.f;
			if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
				rect123.x -= levelWidth - window->w;

			if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
				rect123.y -= Laplas->hitbox.y - window->h / 2.f;
			if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
				rect123.y -= levelHeight - window->h;

			SDL_RenderCopy(ren, Laplas->animation.bullet.texture, NULL, &rect123);
		}
}

void DrawTrapsBullet(mainHero* Laplas, mainWindow* window, int* trapsCount, mainTraps levelTraps[], mainRenderer* trap_dart)
{
	for (int i = 0; i < *trapsCount; i++)
		if (levelTraps[i].shoot.alive)
		{
			SDL_Rect rect123 = { levelTraps[i].shoot.shootAtackCentere.x - TRAP_DART_WIDTH / 2 ,levelTraps[i].shoot.shootAtackCentere.y - TRAP_DART_HIGHT / 2, TRAP_DART_WIDTH, TRAP_DART_HIGHT };

			if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
				rect123.x -= Laplas->hitbox.x - window->w / 2.f;
			if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
				rect123.x -= levelWidth - window->w;

			if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
				rect123.y -= Laplas->hitbox.y - window->h / 2.f;
			if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
				rect123.y -= levelHeight - window->h;

			SDL_RenderCopy(ren, trap_dart->texture, NULL, &rect123);
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

mainTraps* LoadTraps(mainTraps* levelTraps, int* trapsCount, const char levelName[])
{
	FILE* f;
	fopen_s(&f, levelName, "r");

	fscanf_s(f, "%d", trapsCount);

	levelTraps = (mainTraps*)realloc(levelTraps, sizeof(mainTraps) * (*trapsCount));

	for (int i = 0; i < *trapsCount; i++)
	{
		fscanf_s(f, "%d", &levelTraps[i].type);

		fscanf_s(f, "%d", &levelTraps[i].hitbox.x);
		fscanf_s(f, "%d", &levelTraps[i].hitbox.y);
		fscanf_s(f, "%d", &levelTraps[i].hitbox.w);
		fscanf_s(f, "%d", &levelTraps[i].hitbox.h);

		fscanf_s(f, "%d", &levelTraps[i].gazeDirection);
	}



	fclose(f);

	return levelTraps;
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
	Laplas.effect = { HERO_DASH_CD, NULL, HERO_ATACK_CD, NULL, HERO_SHOOT_CD, NULL, CAMERA_SCALE_X, CAMERA_SCALE_Y, NULL, HERO_AFTER_ATACK_PROTECTION, NULL };
	Laplas.animation.com = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas.animation.run = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas.animation.punch = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas.battle = { NULL, { NULL, NULL } , NULL, NULL};
	Laplas.status = { HERO_DAMAGE, HERO_HP, ALIVE , HERO_START_AMUNITION, HERO_SHOOT_DAMAGE};
	Laplas.animationType = NULL;

	return Laplas;
}

void InitEnemys(mainEnemys levelEnemys[], int* enemysCount, mainRenderer* texture_beaver)
{
	for (int i = 0; i < *enemysCount;i++)
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

void InitTraps(mainTraps levelTraps[], int* trapsCount, mainRenderer* texture_dart_trap, mainRenderer* texture_pressure_plate, mainRenderer* texture_trap_spikes)
{
	for (int i = 0; i < *trapsCount; i++)
	{
		if (levelTraps[i].type == 1)
		{
			levelTraps[i].render = *texture_dart_trap;
			levelTraps[i].shoot.alive = NULL;
			levelTraps[i].shoot.bulletSpeed = HERO_BULLET_SPEED;
			levelTraps[i].shoot.shootAtackCentere = { NULL, NULL };
			levelTraps[i].triggered = NULL;
			levelTraps[i].DMG = TRAP_DART_DMG;
		}

		else if (levelTraps[i].type == 2)
		{
			levelTraps[i].render = *texture_pressure_plate;
			levelTraps[i].shoot.alive = NULL;
			levelTraps[i].shoot.bulletSpeed = HERO_BULLET_SPEED;
			levelTraps[i].shoot.shootAtackCentere = { NULL, NULL };
			levelTraps[i].triggered = NULL;
			levelTraps[i].DMG = NULL;
		}

		else if (levelTraps[i].type == 3)
		{
			levelTraps[i].render = *texture_trap_spikes;
			levelTraps[i].shoot.alive = NULL;
			levelTraps[i].shoot.bulletSpeed = HERO_BULLET_SPEED;
			levelTraps[i].shoot.shootAtackCentere = { NULL, NULL };
			levelTraps[i].triggered = NULL;
			levelTraps[i].DMG = TRAP_SPIKES_DMG;
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

void MainMenu(GameState* gameState, mainWindow* window) {
	mainRenderer startButtonTexture;
	mainRenderer optionsButtonTexture;
	mainRenderer creditsButtonTexture;
	mainRenderer exitButtonTexture;
	GetTexture("Textures\\button_start.png", &startButtonTexture, 1);
	GetTexture("Textures\\button_options.png", &optionsButtonTexture, 1);
	GetTexture("Textures\\button_credits.png", &creditsButtonTexture, 1);
	GetTexture("Textures\\button_exit.png", &exitButtonTexture, 1);

	SDL_FRect startButton;
	SDL_FRect settingsButton;
	SDL_FRect creditsButton;
	SDL_FRect exitButton;

	SDL_FRect buttons[4];

	while (true)
	{
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		startButton = { float(window->w / 2.3), float(window->h / 3), float(window->w / 8),float(window->h / 8) };
		SDL_RenderCopyF(ren, startButtonTexture.texture, NULL, &startButton);
		buttons[0] = startButton;

		settingsButton = { float(window->w / 2.3),float(window->h / 2.1),float(window->w / 8),  float(window->h / 8) };
		SDL_RenderCopyF(ren, optionsButtonTexture.texture, NULL, &settingsButton);
		buttons[1] = settingsButton;

		creditsButton = { float(window->w / 2.3),float(window->h / 1.6),float(window->w / 8),float(window->h / 8) };
		SDL_RenderCopyF(ren, creditsButtonTexture.texture, NULL, &creditsButton);
		buttons[2] = creditsButton;

		exitButton = { float(window->w / 2.3) ,float(window->h / 1.3),float(window->w / 8),float(window->h / 8) };
		SDL_RenderCopyF(ren, exitButtonTexture.texture, NULL, &exitButton);
		buttons[3] = exitButton;

		SDL_RenderPresent(ren);
		while (SDL_PollEvent(&ev)) 
		{
			switch (ev.type)
			{
				case SDL_WINDOWEVENT:
					if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						SDL_GetWindowSize(win, &window->w, &window->h);
					}
				break;
				case SDL_MOUSEBUTTONUP:
				{
					SDL_FPoint mousePoint = { ev.button.x,ev.button.y };
					for (int i = 0; i < 4; i++)
					{
						if (SDL_PointInFRect(&mousePoint, &buttons[i])) 
						{
							*gameState = (GameState)i;
							SDL_DestroyTexture(startButtonTexture.texture);
							SDL_DestroyTexture(optionsButtonTexture.texture);
							SDL_DestroyTexture(creditsButtonTexture.texture);
							SDL_DestroyTexture(exitButtonTexture.texture);
							return;
						}
					}
				}
			}
		}
	}
}

void PauseMenu(GameState* gameState, mainWindow* window) {
	
	mainRenderer continueButtonTexture;
	GetTexture("Textures\\button_continue.png", &continueButtonTexture, 1);
	mainRenderer exitButtonTexture;
	GetTexture("Textures\\button_exit.png", &exitButtonTexture, 1);
	SDL_FRect continueButton;
	SDL_FRect exitButton;
	

	


	while (true)
	{
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		continueButton = { float(window->w / 2.3), float(window->h / 3), float(window->w / 8),float(window->h / 8) };
		exitButton = { float(window->w / 2.3),float(window->h / 2.1),float(window->w / 8),  float(window->h / 8) };
		SDL_RenderCopyF(ren, continueButtonTexture.texture, NULL, &continueButton);
		SDL_RenderCopyF(ren, exitButtonTexture.texture, NULL, &exitButton);

		SDL_RenderPresent(ren);
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
				case SDL_WINDOWEVENT:
					if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						SDL_GetWindowSize(win, &window->w, &window->h);
					}
					break;

				case SDL_MOUSEBUTTONUP:
				{
					SDL_FPoint mousePoint = { ev.button.x,ev.button.y };
					if (SDL_PointInFRect(&mousePoint, &continueButton))
					{
						SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
						SDL_RenderClear(ren);
						SDL_DestroyTexture(exitButtonTexture.texture);
						SDL_DestroyTexture(continueButtonTexture.texture);
						*gameState = IN_GAME;
						return;
					}
					else if (SDL_PointInFRect(&mousePoint, &exitButton))
					{
						SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
						SDL_RenderClear(ren);
						SDL_DestroyTexture(exitButtonTexture.texture);
						SDL_DestroyTexture(continueButtonTexture.texture);
						*gameState = MAIN_MENU;
						return;
					}
				}
			}
		}
	}
}

void SettingsMenu(GameState* gameState, Settings*  settings, mainWindow* window) {

	SDL_FRect volumeLeftButton;
	SDL_FRect volumeBar;
	SDL_FRect volumeRightButton;

	mainRenderer plusButtonTexture;
	mainRenderer minusButtonTexture;
	mainRenderer volumeBarTexture;
	mainRenderer ButtonTexture;
	GetTexture("Textures\\button_exit.png", &ButtonTexture, 1);
	GetTexture("Textures\\batton_plus.png", &plusButtonTexture, 1);
	GetTexture("Textures\\button_minus.png", &minusButtonTexture, 1);
	GetTexture("Textures\\volume_bar.png", &volumeBarTexture, 1);

	SDL_FRect skinLeftButton;
	SDL_FRect skinRightButton;

	SDL_FRect exitButton;

	while (true)
	{
		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);

		volumeLeftButton = {float(window->w / 3.12), float(window->h / 3), float(window->w / 25.6), float(window->h / 14.4)};
		volumeRightButton = { float(window->w / 1.66), float(window->h / 3), float(window->w / 25.6), float(window->h / 14.4) };
		volumeBar = { float(window->w / 2.61),float(window->h / 3),float(window->w / 25.6) * settings->volume,float(window->h / 14.4) };

		skinLeftButton = { float(window->w / 3.12), float(window->h / 1.63), float(window->w / 25.6), float(window->h / 14.4) };
		skinRightButton = { float(window->w / 1.66),float(window->h / 1.63), float(window->w / 25.6), float(window->h / 14.4) };

		exitButton = { float(window->w / 2.37),float(window->h / 1.2), float(window->w / 8), float(window->h / 8) };

		SDL_RenderCopyF(ren, minusButtonTexture.texture,NULL,&volumeLeftButton);
		SDL_RenderCopyF(ren, plusButtonTexture.texture, NULL, &volumeRightButton);

		SDL_RenderCopyF(ren, volumeBarTexture.texture, NULL, &volumeBar);

		SDL_RenderCopyF(ren, minusButtonTexture.texture, NULL, &skinLeftButton);
		SDL_RenderCopyF(ren, plusButtonTexture.texture, NULL, &skinRightButton);

		SDL_RenderCopyF(ren, ButtonTexture.texture, NULL, &exitButton);

		SDL_RenderPresent(ren);

		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
				case SDL_WINDOWEVENT:
					if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						SDL_GetWindowSize(win, &window->w, &window->h);
					}
					break;

				case SDL_MOUSEBUTTONUP:
				{
					SDL_FPoint mousePoint = { ev.button.x,ev.button.y };
					if (SDL_PointInFRect(&mousePoint, &volumeLeftButton) && settings->volume > 0) {
						settings->volume--;
					}
					else if (SDL_PointInFRect(&mousePoint, &volumeRightButton) && settings->volume < 5) {
						settings->volume++;
					}

					if (SDL_PointInFRect(&mousePoint, &skinLeftButton) && settings->skin > 0) {
						settings->skin--;
					}
					else if (SDL_PointInFRect(&mousePoint, &skinRightButton) && settings->skin < 5) {
						settings->skin++;
					}

					if (SDL_PointInFRect(&mousePoint, &exitButton))
					{
						SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
						SDL_RenderClear(ren);
						*gameState = MAIN_MENU;

						SDL_DestroyTexture(ButtonTexture.texture);
						SDL_DestroyTexture(plusButtonTexture.texture);
						SDL_DestroyTexture(minusButtonTexture.texture);
						SDL_DestroyTexture(volumeBarTexture.texture);

						return;
					}
				}
			}
		}

		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
	}
}

void CreditsMenu(GameState* gameState, mainWindow* window) {
	
	mainRenderer ButtonTexture;
	GetTexture("Textures\\button_exit.png", &ButtonTexture, 1);
	SDL_FRect exitButton;
	

	while (true)
	{
		while (SDL_PollEvent(&ev))
		{
			SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
			SDL_RenderClear(ren);

			exitButton = { float(window->w / 2.37),float(window->h / 1.2), float(window->w / 8), float(window->h / 8) };
			SDL_RenderCopyF(ren, ButtonTexture.texture, NULL, &exitButton);

			SDL_RenderPresent(ren);
			switch (ev.type)
			{
				case SDL_WINDOWEVENT :
					if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						SDL_GetWindowSize(win, &window->w, &window->h);
					}
				break;

				case SDL_MOUSEBUTTONUP:
				{
					SDL_FPoint mousePoint = { ev.button.x,ev.button.y };

					if (SDL_PointInFRect(&mousePoint, &exitButton))
					{
						SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
						SDL_RenderClear(ren);
						SDL_DestroyTexture(ButtonTexture.texture);
						*gameState = MAIN_MENU;
						return;
					}
				}
			}
		}
	}
}

void DrawLifeBar(mainHero laplas, mainRenderer hpBarTexture, mainRenderer hpBarEdgingTexture, mainWindow* window) {
	SDL_FRect hpBar = { float(window->w / 11), float(window->h) - float(window->h / 4.95), float(window->w / 32.8) * laplas.status.HP / 10,  float(window->h / 14.3) };
	SDL_FRect hpBarEdging = { float(window->w / 30), float(window->h) - float(window->h / 4.15), float(window->w / 2.7),  float(window->h / 7.2) };
	SDL_RenderCopyF(ren, hpBarTexture.texture, NULL, &hpBar);
	SDL_RenderCopyF(ren, hpBarEdgingTexture.texture, NULL, &hpBarEdging);
}

void DrawAmmoBar(mainRenderer ammoBarTexture, mainWindow* window) {
	SDL_FRect ammoBar = { float(window->w / 25.8), float(window->h) - float(window->h / 7.4), float(window->w / 6),  float(window->h / 11.07) };
	SDL_RenderCopyF(ren, ammoBarTexture.texture, NULL, &ammoBar);
}

void HeroBulletHitboxInRange(mainHero* Laplas, int* bordersCount, mainBorders levelBorders[])
{
	for (int f = 0; f < 10; f++)
	{
		if (Laplas->battle.shoot[f].alive)
		{
			for (int g = 0; g < *bordersCount; g++)
				if (HeroPhysicInRange(Laplas->battle.shoot[f].shootAtackCentere, levelBorders[g].bordersHitbox))
					Laplas->battle.shoot[f].alive = 0;
		}
	}
}

void TrapBulletHitboxInRange(mainTraps levelTraps[], int* trapsCount, int* bordersCount, mainBorders levelBorders[])
{
	for (int f = 0; f < *trapsCount; f++)
	{
		if (levelTraps[f].shoot.alive)
		{
			for (int g = 0; g < *bordersCount; g++)
				if (HeroPhysicInRange(levelTraps[f].shoot.shootAtackCentere, levelBorders[g].bordersHitbox))
					levelTraps[f].shoot.alive = 0;
		}
	}
}

void HeroBulletOutworldCheck(mainHero* Laplas, mainBorders levelBorders[])
{
	for(int i = 0; i < 10; i++)
		if (Laplas->battle.shoot[i].alive)
		{
			if (Laplas->battle.shoot[i].shootAtackCentere.x > levelBorders[1].bordersHitbox.w - HERO_BULLET_WIDTH / 2 - 2)
				Laplas->battle.shoot[i].alive = 0;
			else if (Laplas->battle.shoot[i].shootAtackCentere.x < levelBorders[0].bordersHitbox.x + HERO_BULLET_WIDTH / 2 + 2)
				Laplas->battle.shoot[i].alive = 0;

			if (Laplas->battle.shoot[i].shootAtackCentere.y > levelBorders[0].bordersHitbox.h - HERO_BULLET_HIGHT / 2 - 2)
				Laplas->battle.shoot[i].alive = 0;
			else if (Laplas->battle.shoot[i].shootAtackCentere.y < levelBorders[0].bordersHitbox.y + HERO_BULLET_HIGHT / 2 + 2)
				Laplas->battle.shoot[i].alive = 0;
		}
}

void TrapBulletOutworldCheck(mainTraps levelTraps[], int* trapsCount, mainBorders levelBorders[])
{
	for (int f = 0; f < *trapsCount; f++)
		if (levelTraps[f].shoot.alive)
		{
			if (levelTraps[f].shoot.shootAtackCentere.x > levelBorders[1].bordersHitbox.w - HERO_BULLET_WIDTH / 2 - 2)
				levelTraps[f].shoot.alive = 0;
			else if (levelTraps[f].shoot.shootAtackCentere.x < levelBorders[0].bordersHitbox.x + HERO_BULLET_WIDTH / 2 + 2)
				levelTraps[f].shoot.alive = 0;

			if (levelTraps[f].shoot.shootAtackCentere.y > levelBorders[0].bordersHitbox.h - HERO_BULLET_HIGHT / 2 - 2)
				levelTraps[f].shoot.alive = 0;
			else if (levelTraps[f].shoot.shootAtackCentere.y < levelBorders[0].bordersHitbox.y + HERO_BULLET_HIGHT / 2 + 2)
				levelTraps[f].shoot.alive = 0;
		}
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
	mainRenderer texture_trap_with_dart;
	mainRenderer texture_pressure_plate;
	mainRenderer texture_trap_dart;
	mainRenderer texture_trap_spikes;
	TTF_Font* fontNovemBig = NULL;
	TTF_Font* fontNovemSmall = NULL;

	
	mainRenderer hpBarTexture;
	mainRenderer hpBarEdgingTexture;
	mainRenderer ammoBarTexture;

	int bordersCount = NULL;
	int enemysCount = NULL;
	int trapsCount = NULL;
	int lastTime = 0;
	int temp = 0;
	int deltaTime = clock();
	int check = 1;

	mainBorders* levelBorders = NULL;
	mainEnemys* levelEnemys = NULL;
	mainTraps* levelTraps = NULL;
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
	GetTexture("Textures\\hero_bullet.png", &Laplas.animation.bullet, 1);
	Laplas.hitbox.h *= (Laplas.animation.com.frame.w / 1. / Laplas.animation.com.frame.h);

	GetTexture("Textures\\bobr.png", &texture_beaver, 6);

	GetTexture("Textures\\BackGroundCave.png", &texture_backGround, 1);

	GetTexture("Textures\\cobblestone40x40.png", &texture_cobbleStone, 1);

	GetTexture("Textures\\woodenPlatform.png", &texture_platform, 1);
	
	GetTexture("Textures\\trap_with_dart.png", &texture_trap_with_dart, 1);

	GetTexture("Textures\\pressure_plate.png", &texture_pressure_plate, 1);

	GetTexture("Textures\\trap_dart.png", &texture_trap_dart, 1);

	GetTexture("Textures\\trap_spikes.png", &texture_trap_spikes, 1);




	GetTexture("Textures\\life_bar.png", &hpBarTexture, 1);
	GetTexture("Textures\\outside_life_bar.png", &hpBarEdgingTexture, 1);
	GetTexture("Textures\\ammo_bar.png", &ammoBarTexture, 1);
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

	levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Levels\\Borders.txt");
	levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemys\\Enemy.txt");
	levelTraps = LoadTraps(levelTraps, &trapsCount, "Traps\\Trap.txt");

	InitEnemys(levelEnemys, &enemysCount, &texture_beaver);
	InitTraps(levelTraps, &trapsCount, &texture_trap_with_dart, &texture_pressure_plate, &texture_trap_spikes);


	while (flag)
	{
		switch (gameState)
		{
		case MAIN_MENU:
			MainMenu(&gameState, &window);
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
				HeroPhysicHitboxOverlay(&bordersCount, &Laplas, levelBorders, &trapsCount, levelTraps);
				EnemyPhysicHitboxOverlay(&bordersCount, &enemysCount, levelEnemys, levelBorders);

				//Проверка пуль на касание с стеной
				TrapBulletHitboxInRange(levelTraps, &trapsCount, &bordersCount, levelBorders);
				HeroBulletHitboxInRange(&Laplas, &bordersCount, levelBorders);

				//Выход за границы мира
				HeroPhysicOutworldCheck(&Laplas, levelBorders);
				EnemyPhysicOutworldCheck(&enemysCount, levelEnemys, levelBorders);
				HeroBulletOutworldCheck(&Laplas, levelBorders);
				TrapBulletOutworldCheck(levelTraps, &trapsCount, levelBorders);


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
					levelBorders = LoadLevel(levelBorders, &bordersCount, &Laplas, "Levels\\Borders1.txt");
					levelEnemys = LoadEnemys(levelEnemys, &enemysCount, "Enemys\\Enemy1.txt");
					InitEnemys(levelEnemys, &enemysCount, &texture_beaver);
				}

				#pragma endregion 

				#pragma region BATTLE

				HeroCommonAtack(&Laplas, &deltaTime, &enemysCount, levelEnemys);

				HeroShootAtack(&Laplas, &deltaTime, &enemysCount, levelEnemys);

				for (int t = 0; t < trapsCount; t++)
					if (levelTraps[t].type == 2 && levelTraps[t].triggered && !levelTraps[t - 1].shoot.alive)
					{
						if (levelTraps[t - 1].gazeDirection == 0)
						{
							levelTraps[t - 1].shoot.shootAtackCentere.x = levelTraps[t - 1].hitbox.x + levelTraps[t - 1].hitbox.w / 2;
							levelTraps[t - 1].shoot.shootAtackCentere.y = levelTraps[t - 1].hitbox.y;
							levelTraps[t - 1].shoot.bulletSpeed = HERO_BULLET_SPEED;
							levelTraps[t - 1].shoot.alive = 1;
						}
						else
						{
							levelTraps[t - 1].shoot.shootAtackCentere.x = levelTraps[t - 1].hitbox.x - levelTraps[t - 1].hitbox.w / 2;
							levelTraps[t - 1].shoot.shootAtackCentere.y = levelTraps[t - 1].hitbox.y;
							levelTraps[t - 1].shoot.bulletSpeed = -HERO_BULLET_SPEED;
							levelTraps[t - 1].shoot.alive = 1;
						}
					}
					else if (levelTraps[t].type == 3 && levelTraps[t].triggered)
					{
						Laplas.status.HP -= levelTraps[t].DMG;
					}
					
				
				//Атака ловушкой по ГГ
				for (int j = 0; j < trapsCount; j++)
					if (levelTraps[j].shoot.alive)
					{
						levelTraps[j].shoot.shootAtackCentere.x += levelTraps[j].shoot.bulletSpeed;
						
						if (!Laplas.effect.underAtack && CheckShootHitbox(&levelTraps[j].shoot.shootAtackCentere, &Laplas.hitbox))
						{
							levelTraps[j].shoot.alive = 0;
							Laplas.effect.underAtack = 1;
							Laplas.effect.lastDamage = deltaTime;
							Laplas.status.HP -= levelTraps[j].DMG;
							if (Laplas.status.HP <= 0)
							{
								Laplas.status.alive = 0;
							}
						}
						
					}

				if (Laplas.effect.lastDamage + Laplas.effect.afterAtackResist < deltaTime)
					Laplas.effect.underAtack = 0;



				#pragma endregion 

				#pragma region DRAW

				//Задний фон
				SDL_RenderCopy(ren, texture_backGround.texture, NULL, NULL);
		
				//Враги и стены
				DrawHitbox(bordersCount, levelBorders, &Laplas, &window, &texture_cobbleStone, &texture_platform, &texture_trap_with_dart);
				DrawEnemys(&enemysCount, levelEnemys, &Laplas, &window);
				DrawTraps(&trapsCount, levelTraps, &Laplas, &window);
				
				//Отрисовка таймера
				if (lastTime != deltaTime / 1000 % 60)
				{
					lastTime = deltaTime / 1000 % 60;
					sprintf_s(timer_text, "%02i:%02i", deltaTime / 60000, lastTime);
					SDL_DestroyTexture(texture_timer.texture);
					texture_timer = CreateTimerFromText(timer_text, fontNovemBig, { 100, 100, 100, 255 });
				}

				//Отрисовка кол-ва боеприпасов
				sprintf_s(amunition_text, "%03i", Laplas.status.HP);
				SDL_DestroyTexture(texture_ammunition.texture);
				texture_ammunition = CreateAmmunitionFromText(amunition_text, fontNovemSmall, { 255, 215, 0, 255 });

		
				//Таймер и боезапас
				SDL_RenderCopy(ren, texture_timer.texture, NULL, &texture_timer.textureSize);
				SDL_RenderCopy(ren, texture_ammunition.texture, NULL, &texture_ammunition.textureSize);

				//Отрисовка пуль
				DrawHeroBullet(&Laplas, &window);
				DrawTrapsBullet(&Laplas, &window, &trapsCount, levelTraps, &texture_trap_dart);

				//ГГ
				DrawMainHero(&Laplas, window);

				DrawAmmoBar(ammoBarTexture, &window);
				DrawLifeBar(Laplas,hpBarTexture, hpBarEdgingTexture, &window);

				SDL_RenderPresent(ren);

				//Очистка
				SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
				SDL_RenderClear(ren);
				#pragma endregion 

				FPSControl();
			}
			break;

		case CREDITS:
			CreditsMenu(&gameState, &window);
			break;

		case SETTINGS:
			SettingsMenu(&gameState,&settings, &window);
			break;

		case PAUSE_MENU:
			PauseMenu(&gameState, &window);
			break;

		case QUIT:
			flag = 0;
			break;
		}
		

	}



	

	free(levelBorders);
	free(levelEnemys);
	free(levelTraps);
	TTF_CloseFont(fontNovemBig);
	TTF_CloseFont(fontNovemSmall);

	SDL_DestroyTexture(texture_backGround.texture);
	SDL_DestroyTexture(texture_cobbleStone.texture);
	SDL_DestroyTexture(texture_timer.texture);
	SDL_DestroyTexture(texture_beaver.texture);
	SDL_DestroyTexture(texture_platform.texture);
	SDL_DestroyTexture(texture_trap_with_dart.texture);
	SDL_DestroyTexture(texture_pressure_plate.texture);
	SDL_DestroyTexture(texture_trap_dart.texture);
	SDL_DestroyTexture(texture_trap_spikes.texture);


	SDL_DestroyTexture(Laplas.animation.com.texture);
	SDL_DestroyTexture(Laplas.animation.bullet.texture);
	SDL_DestroyTexture(Laplas.animation.punch.texture);
	SDL_DestroyTexture(Laplas.animation.run.texture);
	SDL_DestroyTexture(Laplas.animation.shoot.texture);
	
	
	SDL_DestroyTexture(texture_platform.texture);
	SDL_DestroyTexture(hpBarEdgingTexture.texture);
	SDL_DestroyTexture(hpBarTexture.texture);
	SDL_DestroyTexture(ammoBarTexture.texture);

	DeInit(0, &win, &ren, &win_surface);
	
	return 0;
}
