#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "Audio.h"
#include "common_parameters.h"
#include <time.h>



void DrawMainHero(mainHero* Laplas, mainWindow window, SDL_Renderer *ren, int levelWidth, int levelHeight)
{
	SDL_SetRenderDrawColor(ren, 255, 0, 128, 255);
	SDL_Rect movedLaplas = { window.w / 2.f ,window.h / 2.f, Laplas->hitbox.w, Laplas->hitbox.h };

	if (Laplas->hitbox.x <= window.w / 2.f)
		movedLaplas.x = Laplas->hitbox.x;
	if (Laplas->hitbox.x > levelWidth - window.w / 2.f)
		movedLaplas.x = Laplas->hitbox.x - (levelWidth - window.w);

	if (Laplas->hitbox.y <= window.h / 2.f)
		movedLaplas.y = Laplas->hitbox.y;
	if (Laplas->hitbox.y > levelHeight - window.h / 2.f)
		movedLaplas.y = Laplas->hitbox.y - (levelHeight - window.h);

	Laplas->texture_rect.x = movedLaplas.x;//- (Laplas->texture_rect.w / 2.)
	Laplas->texture_rect.y = movedLaplas.y;//+ Laplas->hitbox.h / 2. - (Laplas->texture_rect.h)

	movedLaplas.x -= Laplas->hitbox.w / 2.;
	movedLaplas.y -= Laplas->hitbox.h / 2.;

	//SDL_RenderFillRect(ren, &movedLaplas);

	if (Laplas->physic.xMoveL + Laplas->physic.xMoveR == 0 && !Laplas->battle.commonAtack && !Laplas->battle.shootAtack)
		Laplas->animationType = 0;
	else if (!Laplas->battle.commonAtack && !Laplas->battle.shootAtack)
		Laplas->animationType = 1;

	switch (Laplas->animationType)
	{
	case 0:
		Laplas->texture_rect.x += -(Laplas->texture_rect.w / 2.);
		Laplas->texture_rect.y += Laplas->hitbox.h / 2. - (Laplas->texture_rect.h);
		if (Laplas->physic.gazeDirection < 0)
			SDL_RenderCopyEx(ren, Laplas->animation.com.texture, &Laplas->animation.com.frame, &Laplas->texture_rect, 0, 0, SDL_FLIP_HORIZONTAL);
		else if (Laplas->physic.gazeDirection > 0)
			SDL_RenderCopyEx(ren, Laplas->animation.com.texture, &Laplas->animation.com.frame, &Laplas->texture_rect, 0, 0, SDL_FLIP_NONE);
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
		{
			Laplas->texture_rect.x += Laplas->hitbox.w / 2. - (Laplas->texture_rect.w / 2.);
			Laplas->texture_rect.y += Laplas->hitbox.h / 2. - (Laplas->texture_rect.h);
			SDL_RenderCopyEx(ren, Laplas->animation.run.texture, &Laplas->animation.run.frame, &Laplas->texture_rect, 0, 0, SDL_FLIP_HORIZONTAL);
			Laplas->animation.ball.angel -= 5;
		}
		else if (Laplas->physic.gazeDirection > 0)
		{
			Laplas->texture_rect.x += -(Laplas->texture_rect.w / 1.5);
			Laplas->texture_rect.y += Laplas->hitbox.h / 2. - (Laplas->texture_rect.h);
			SDL_RenderCopyEx(ren, Laplas->animation.run.texture, &Laplas->animation.run.frame, &Laplas->texture_rect, 0, 0, SDL_FLIP_NONE);
			Laplas->animation.ball.angel += 5;
		}

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
		{
			Laplas->texture_rect.x += -(Laplas->texture_rect.w / 2.);
			Laplas->texture_rect.y += Laplas->hitbox.h / 1.63 - (Laplas->texture_rect.h);
			SDL_RenderCopyEx(ren, Laplas->animation.punch.texture, &Laplas->animation.punch.frame, &Laplas->texture_rect, 0, 0, SDL_FLIP_HORIZONTAL);
		}
		else if (Laplas->physic.gazeDirection > 0)
		{
			Laplas->texture_rect.x += -(Laplas->texture_rect.w / 2.);
			Laplas->texture_rect.y += Laplas->hitbox.h / 1.63 - (Laplas->texture_rect.h);
			SDL_RenderCopyEx(ren, Laplas->animation.punch.texture, &Laplas->animation.punch.frame, &Laplas->texture_rect, 0, 0, SDL_FLIP_NONE);
		}
		if (SDL_GetTicks() - Laplas->animation.punch.frameTime > HERO_ATACK_CD / Laplas->animation.punch.frameCount)
		{
			Laplas->animation.punch.frame.x += Laplas->animation.punch.textureSize.w / Laplas->animation.punch.frameCount;
			Laplas->animation.punch.frameTime = SDL_GetTicks();
		}

		if (Laplas->animation.punch.frame.x >= Laplas->animation.punch.textureSize.w)
			Laplas->animation.punch.frame.x = 0;
		break;

	case 3:
		if (Laplas->physic.gazeDirection < 0)
		{
			Laplas->texture_rect.x += -(Laplas->texture_rect.w / 1.5);
			Laplas->texture_rect.y += Laplas->hitbox.h / 2. - (Laplas->texture_rect.h);
			SDL_RenderCopyEx(ren, Laplas->animation.shoot.texture, &Laplas->animation.shoot.frame, &Laplas->texture_rect, 0, 0, SDL_FLIP_HORIZONTAL);
		}
		else if (Laplas->physic.gazeDirection > 0)
		{
			Laplas->texture_rect.x += Laplas->hitbox.w / 2. - (Laplas->texture_rect.w / 2.);
			Laplas->texture_rect.y += Laplas->hitbox.h / 2. - (Laplas->texture_rect.h);
			SDL_RenderCopyEx(ren, Laplas->animation.shoot.texture, &Laplas->animation.shoot.frame, &Laplas->texture_rect, 0, 0, SDL_FLIP_NONE);
		}
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

void DrawHitbox(int bordersCount, mainBorders levelBorders[], mainHero* Laplas, mainWindow* window, mainRenderer* cobbleStone, 
	mainRenderer* platform, mainRenderer* trampline, mainRenderer* tmp_Platform, SDL_Renderer* ren, int levelWidth, int levelHeight)
{
	SDL_Rect rect123;
	SDL_Rect dopRect = { NULL, NULL, NULL, NULL };
	bool check = 1;
	for (int i = 0; i < bordersCount; i++)
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
		case 2:

			SDL_Rect rect1 = cobbleStone->textureSize;
			dopRect = cobbleStone->textureSize;

			for (int i = rect123.y; i < rect123.y + rect123.h; i += cobbleStone->textureSize.h)
				for (int j = rect123.x; j < rect123.x + rect123.w; j += cobbleStone->textureSize.w)
				{
					if (j + cobbleStone->textureSize.w > rect123.x + rect123.w)
						dopRect.w = rect123.x + rect123.w - j;
					else
						dopRect.w = cobbleStone->textureSize.w;

					if (i + cobbleStone->textureSize.h > rect123.y + rect123.h)
						dopRect.h = rect123.y + rect123.h - i;
					else
						dopRect.h = cobbleStone->textureSize.h;

					rect1.x = j;
					rect1.y = i;
					rect1.w = dopRect.w;
					rect1.h = dopRect.h;
					SDL_RenderCopy(ren, cobbleStone->texture, &dopRect, &rect1);

				}
			break;

		case 4:
			SDL_RenderCopy(ren, platform->texture, NULL, &rect123);
			break;

		case 5:
			SDL_RenderCopy(ren, trampline->texture, NULL, &rect123);
			break;

		case 7:
			SDL_RenderDrawRect(ren, &rect123);
			if(levelBorders[i].alive)
				SDL_RenderCopy(ren, tmp_Platform->texture, NULL, &rect123);
			break;

		}
	}
}

void DrawFakeWalls(int bordersCount, mainBorders levelBorders[], mainHero* Laplas, mainWindow* window, mainRenderer* cobbleStone
	, SDL_Renderer* ren, int levelWidth, int levelHeight)
{
	SDL_Rect rect123;
	SDL_Rect dopRect = { NULL, NULL, NULL, NULL };
	bool check = 1;
	SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);


	for (int i = 0; i < bordersCount; i++)
		if (levelBorders[i].type == 6)
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

			SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
			SDL_RenderFillRect(ren, &rect123);
			SDL_Rect rect1 = cobbleStone->textureSize;
			dopRect = cobbleStone->textureSize;

			for (int i = rect123.y; i < rect123.y + rect123.h; i += cobbleStone->textureSize.h)
				for (int j = rect123.x; j < rect123.x + rect123.w; j += cobbleStone->textureSize.w)
				{
					if (j + cobbleStone->textureSize.w > rect123.x + rect123.w)
						dopRect.w = rect123.x + rect123.w - j;
					else
						dopRect.w = cobbleStone->textureSize.w;

					if (i + cobbleStone->textureSize.h > rect123.y + rect123.h)
						dopRect.h = rect123.y + rect123.h - i;
					else
						dopRect.h = cobbleStone->textureSize.h;

					rect1.x = j;
					rect1.y = i;
					rect1.w = dopRect.w;
					rect1.h = dopRect.h;
					SDL_RenderCopy(ren, cobbleStone->texture, &dopRect, &rect1);

				}

		}
}

void DrawBuffsEffect(SDL_Renderer* ren, mainHero* Laplas, int levelWidth, int levelHeight, mainWindow* window)
{
	SDL_Rect movedEffect = { -window->w + Laplas->hitbox.x, -window->h+Laplas->hitbox.y, window->w*2, window->h*2};

	if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
		movedEffect.x -= Laplas->hitbox.x - window->w / 2.f;
	if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
		movedEffect.x -= levelWidth - window->w;

	if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
		movedEffect.y -= Laplas->hitbox.y - window->h / 2.f;
	if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
		movedEffect.y -= levelHeight - window->h;

	if(Laplas->buffs.buffDMGactive)
		SDL_RenderCopy(ren, Laplas->animation.DMG_Buff.texture, NULL, &movedEffect);
}

void DrawItemsEffect(SDL_Renderer* ren, mainHero* Laplas, int levelWidth, int levelHeight, mainWindow* window)
{
	if (Laplas->buffs.itemBallActive)
	{
		SDL_Rect movedItem = {Laplas->hitbox.x - Laplas->hitbox.w * 1.6, Laplas->hitbox.y - Laplas->hitbox.h*1.3, HERO_HEIGHT*2.1, HERO_HEIGHT*2.1 };

		if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
			movedItem.x -= Laplas->hitbox.x - window->w / 2.f;
		if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
			movedItem.x -= levelWidth - window->w;

		if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
			movedItem.y -= Laplas->hitbox.y - window->h / 2.f;
		if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
			movedItem.y -= levelHeight - window->h;

		SDL_RenderCopyEx(ren, Laplas->animation.ball.texture, NULL, &movedItem, Laplas->animation.ball.angel, NULL, SDL_FLIP_NONE);
	}
		
}

void DrawEnemys(int* enemysCount, mainEnemys levelEnemys[], mainHero* Laplas, mainWindow* window, mainRenderer* , SDL_Renderer* ren, 
	int levelWidth, int levelHeight)
{
	SDL_SetRenderDrawColor(ren, 128, 255, 128, 255);
	for (int i = 0; i < *enemysCount; i++)
	{
		SDL_Rect movedEnemy = { levelEnemys[i].hitbox.x - levelEnemys[i].hitbox.w / 2,levelEnemys[i].hitbox.y - levelEnemys[i].hitbox.h / 2, levelEnemys[i].hitbox.w, levelEnemys[i].hitbox.h };

		if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
			movedEnemy.x -= Laplas->hitbox.x - window->w / 2.f;
		if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
			movedEnemy.x -= levelWidth - window->w;

		if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
			movedEnemy.y -= Laplas->hitbox.y - window->h / 2.f;
		if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
			movedEnemy.y -= levelHeight - window->h;


		//SDL_RenderFillRect(ren, &movedEnemy);

		switch (levelEnemys[i].type)
		{
		case 1:
			switch (levelEnemys[i].animation_type)
			{
			case 1:
				if (levelEnemys[i].physic.gazeDirection > 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, levelEnemys[i].animation.run.angel, 0, SDL_FLIP_HORIZONTAL);
				else if (levelEnemys[i].physic.gazeDirection < 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, levelEnemys[i].animation.run.angel, 0, SDL_FLIP_NONE);
				else if (levelEnemys[i].physic.gazeDirection == 0)
				{
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
					break;
				}
				//levelEnemys[i].animation.run.angel += 10;

				if ((levelEnemys[i].physic.xMoveL || levelEnemys[i].physic.xMoveR) && (SDL_GetTicks() - levelEnemys[i].animation.run.frameTime > 1000 / 30) && (levelEnemys[i].physic.xMoveL + levelEnemys[i].physic.xMoveR != 0))
				{
					levelEnemys[i].animation.run.frame.x += levelEnemys[i].animation.run.textureSize.w / levelEnemys[i].animation.run.frameCount;
					levelEnemys[i].animation.run.frameTime = SDL_GetTicks();
				}

				if (levelEnemys[i].animation.run.frame.x >= levelEnemys[i].animation.run.textureSize.w || (!levelEnemys[i].physic.xMoveL && !levelEnemys[i].physic.xMoveR) || (levelEnemys[i].physic.xMoveL + levelEnemys[i].physic.xMoveR == 0))
					levelEnemys[i].animation.run.frame.x = 0;

				break;

			case 6:
				if (levelEnemys[i].physic.gazeDirection > 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.preAtack.texture, &levelEnemys[i].animation.preAtack.frame, &movedEnemy, levelEnemys[i].animation.preAtack.angel, 0, SDL_FLIP_HORIZONTAL);
				else if (levelEnemys[i].physic.gazeDirection < 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.preAtack.texture, &levelEnemys[i].animation.preAtack.frame, &movedEnemy, levelEnemys[i].animation.preAtack.angel, 0, SDL_FLIP_NONE);
				else if (levelEnemys[i].physic.gazeDirection == 0)
				{
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.preAtack.texture, &levelEnemys[i].animation.preAtack.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
					break;
				}
				break;
			}
			break;


		case 2:
			if (levelEnemys[i].physic.gazeDirection > 0)
				SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
			else if (levelEnemys[i].physic.gazeDirection < 0)
				SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
			else if (levelEnemys[i].physic.gazeDirection == 0)
			{
				SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
				break;
			}

			if ((levelEnemys[i].physic.xMoveL || levelEnemys[i].physic.xMoveR) && (SDL_GetTicks() - levelEnemys[i].animation.run.frameTime > 1000 / 15) && (levelEnemys[i].physic.xMoveL + levelEnemys[i].physic.xMoveR != 0))
			{
				levelEnemys[i].animation.run.frame.x += levelEnemys[i].animation.run.textureSize.w / levelEnemys[i].animation.run.frameCount;
				levelEnemys[i].animation.run.frameTime = SDL_GetTicks();
			}

			if (levelEnemys[i].animation.run.frame.x >= levelEnemys[i].animation.run.textureSize.w || (!levelEnemys[i].physic.xMoveL && !levelEnemys[i].physic.xMoveR) || (levelEnemys[i].physic.xMoveL + levelEnemys[i].physic.xMoveR == 0))
				levelEnemys[i].animation.run.frame.x = 0;
			break;


		case 3:
			if (levelEnemys[i].physic.gazeDirection > 0)
				SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_HORIZONTAL);
			else if (levelEnemys[i].physic.gazeDirection < 0)
				SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
			else if (levelEnemys[i].physic.gazeDirection == 0)
			{
				SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
				break;
			}

			if ((levelEnemys[i].physic.xMoveL || levelEnemys[i].physic.xMoveR) && (SDL_GetTicks() - levelEnemys[i].animation.run.frameTime > 1000 / 15) && (levelEnemys[i].physic.xMoveL + levelEnemys[i].physic.xMoveR != 0))
			{
				levelEnemys[i].animation.run.frame.x += levelEnemys[i].animation.run.textureSize.w / levelEnemys[i].animation.run.frameCount;
				levelEnemys[i].animation.run.frameTime = SDL_GetTicks();
			}

			if (levelEnemys[i].animation.run.frame.x >= levelEnemys[i].animation.run.textureSize.w || (!levelEnemys[i].physic.xMoveL && !levelEnemys[i].physic.xMoveR) || (levelEnemys[i].physic.xMoveL + levelEnemys[i].physic.xMoveR == 0))
				levelEnemys[i].animation.run.frame.x = 0;
			break;

		case 4:
			if (levelEnemys[i].physic.gazeDirection > 0)
				SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_HORIZONTAL);
			else if (levelEnemys[i].physic.gazeDirection < 0)
				SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
			else if (levelEnemys[i].physic.gazeDirection == 0)
			{
				SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
				break;
			}
			break;

		case 5:

			if (levelEnemys[i].animation_type == 6) {
				if (levelEnemys[i].physic.gazeDirection > 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.atack.texture, &levelEnemys[i].animation.atack.frame, &movedEnemy, levelEnemys[i].animation.atack.angel, 0, SDL_FLIP_NONE);
				else if (levelEnemys[i].physic.gazeDirection < 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.atack.texture, &levelEnemys[i].animation.atack.frame, &movedEnemy, levelEnemys[i].animation.atack.angel, 0, SDL_FLIP_NONE);
				else if (levelEnemys[i].physic.gazeDirection == 0)
				{
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.atack.texture, &levelEnemys[i].animation.atack.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
					break;
				}
	
				

				if ( (SDL_GetTicks() - levelEnemys[i].animation.atack.frameTime > 1000 / 15))
				{
					levelEnemys[i].animation.atack.frame.x += levelEnemys[i].animation.atack.textureSize.w / levelEnemys[i].animation.atack.frameCount;
					levelEnemys[i].animation.atack.frameTime = SDL_GetTicks();
				}

				if (levelEnemys[i].animation.atack.frame.x >= levelEnemys[i].animation.atack.textureSize.w )
					levelEnemys[i].animation.atack.frame.x = 0;

			}
			else if (levelEnemys[i].animation_type == 1) {
				if (levelEnemys[i].physic.gazeDirection > 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.preAtack.texture, &levelEnemys[i].animation.preAtack.frame, &movedEnemy, levelEnemys[i].animation.preAtack.angel, 0, SDL_FLIP_HORIZONTAL);
				else if (levelEnemys[i].physic.gazeDirection < 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.preAtack.texture, &levelEnemys[i].animation.preAtack.frame, &movedEnemy, levelEnemys[i].animation.preAtack.angel, 0, SDL_FLIP_NONE);
				else if (levelEnemys[i].physic.gazeDirection == 0)
				{
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.preAtack.texture, &levelEnemys[i].animation.preAtack.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
					break;
				}
				
				if (SDL_GetTicks() - levelEnemys[i].animation.preAtack.frameTime > 1000 / 15)
				{
					levelEnemys[i].animation.preAtack.frame.x += levelEnemys[i].animation.preAtack.textureSize.w / levelEnemys[i].animation.preAtack.frameCount;
					levelEnemys[i].animation.preAtack.frameTime = SDL_GetTicks();
				}

				if (levelEnemys[i].animation.preAtack.frame.x >= levelEnemys[i].animation.preAtack.textureSize.w )
					levelEnemys[i].animation.preAtack.frame.x = 0;
			}
			break;

		case 6:
			if (levelEnemys[i].animation_type == 1)
			{
				if (levelEnemys[i].physic.gazeDirection > 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, levelEnemys[i].animation.run.angel, 0, SDL_FLIP_HORIZONTAL);
				else if (levelEnemys[i].physic.gazeDirection < 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, levelEnemys[i].animation.run.angel, 0, SDL_FLIP_NONE);
				else if (levelEnemys[i].physic.gazeDirection == 0)
				{
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.run.texture, &levelEnemys[i].animation.run.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
				}

				if (SDL_GetTicks() - levelEnemys[i].animation.run.frameTime > 1000 / 15)
				{
					levelEnemys[i].animation.run.frame.x += levelEnemys[i].animation.run.textureSize.w / levelEnemys[i].animation.run.frameCount;
					levelEnemys[i].animation.run.frameTime = SDL_GetTicks();
				}

				if (levelEnemys[i].animation.run.frame.x >= levelEnemys[i].animation.run.textureSize.w)
					levelEnemys[i].animation.run.frame.x = 0;
			}

			else if (levelEnemys[i].animation_type == 6)
			{
				if (levelEnemys[i].physic.gazeDirection > 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.atack.texture, &levelEnemys[i].animation.atack.frame, &movedEnemy, levelEnemys[i].animation.atack.angel, 0, SDL_FLIP_HORIZONTAL);
				else if (levelEnemys[i].physic.gazeDirection < 0)
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.atack.texture, &levelEnemys[i].animation.atack.frame, &movedEnemy, levelEnemys[i].animation.atack.angel, 0, SDL_FLIP_NONE);
				else if (levelEnemys[i].physic.gazeDirection == 0)
				{
					SDL_RenderCopyEx(ren, levelEnemys[i].animation.atack.texture, &levelEnemys[i].animation.atack.frame, &movedEnemy, 0, 0, SDL_FLIP_NONE);
				}

				if (SDL_GetTicks() - levelEnemys[i].animation.atack.frameTime > 1000 / 15)
				{
					levelEnemys[i].animation.atack.frame.x += levelEnemys[i].animation.atack.textureSize.w / levelEnemys[i].animation.atack.frameCount;
					levelEnemys[i].animation.atack.frameTime = SDL_GetTicks();
				}

				if (levelEnemys[i].animation.atack.frame.x >= levelEnemys[i].animation.atack.textureSize.w)
					levelEnemys[i].animation.atack.frame.x = 0;
			}


			break;
		}

		if (levelEnemys[i].effect.poisoned)
		{
			SDL_RenderCopy(ren, levelEnemys[i].animation.acid_effect.texture, &levelEnemys[i].animation.acid_effect.frame, &movedEnemy);

			if (SDL_GetTicks() - levelEnemys[i].animation.acid_effect.frameTime > 1000 / 15)
			{
				levelEnemys[i].animation.acid_effect.frame.x += levelEnemys[i].animation.acid_effect.textureSize.w / levelEnemys[i].animation.acid_effect.frameCount;
				levelEnemys[i].animation.acid_effect.frameTime = SDL_GetTicks();
			}

			if (levelEnemys[i].animation.acid_effect.frame.x >= levelEnemys[i].animation.acid_effect.textureSize.w )
				levelEnemys[i].animation.acid_effect.frame.x = 0;
		}

		if (levelEnemys[i].type==4 && levelEnemys[i].shoot.alive)
		{
			SDL_Rect movedEnemy = { levelEnemys[i].shoot.shootAtackCentere.x - levelEnemys[i].animation.bullet.textureSize.w/2,levelEnemys[i].shoot.shootAtackCentere.y - levelEnemys[i].animation.bullet.textureSize.h / 2, levelEnemys[i].animation.bullet.textureSize.w, levelEnemys[i].animation.bullet.textureSize.h };

			if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
				movedEnemy.x -= Laplas->hitbox.x - window->w / 2.f;
			if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
				movedEnemy.x -= levelWidth - window->w;

			if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
				movedEnemy.y -= Laplas->hitbox.y - window->h / 2.f;
			if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
				movedEnemy.y -= levelHeight - window->h;

			SDL_RenderCopy(ren, levelEnemys[i].animation.bullet.texture, NULL, &movedEnemy);
		}

		if (levelEnemys[i].type == 6 && levelEnemys[i].shoot.alive)
		{
			SDL_Rect movedEnemy = { levelEnemys[i].shoot.shootAtackCentere.x - levelEnemys[i].animation.bullet.textureSize.w / 2,
				levelEnemys[i].shoot.shootAtackCentere.y - levelEnemys[i].animation.bullet.textureSize.h / 2, 
				levelEnemys[i].animation.bullet.textureSize.w, 
				levelEnemys[i].animation.bullet.textureSize.h };

			if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
				movedEnemy.x -= Laplas->hitbox.x - window->w / 2.f;
			if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
				movedEnemy.x -= levelWidth - window->w;

			if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
				movedEnemy.y -= Laplas->hitbox.y - window->h / 2.f;
			if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
				movedEnemy.y -= levelHeight - window->h;

			SDL_RenderCopy(ren, levelEnemys[i].animation.bullet.texture, NULL, &movedEnemy);
		}
	}
}

void DrawItem(mainHero* Laplas, mainItems items[], mainWindow* window, SDL_Renderer* ren, int levelWidth, int levelHeight, 
	int* itemsCount, TTF_Font* font)
{
	
	SDL_Surface* surface = NULL;
	mainRenderer texture_cost;
	char cost_text[10] = "000";
	for (int i = 0; i < *itemsCount; i++)
	{
		if (items[i].alive)
		{
			SDL_Rect rect123 = items[i].hitbox;

			if (Laplas->hitbox.x >= window->w / 2.f && Laplas->hitbox.x <= levelWidth - window->w / 2.f)
				rect123.x -= Laplas->hitbox.x - window->w / 2.f;
			if (Laplas->hitbox.x > levelWidth - window->w / 2.f)
				rect123.x -= levelWidth - window->w;

			if (Laplas->hitbox.y >= window->h / 2.f && Laplas->hitbox.y <= levelHeight - window->h / 2.f)
				rect123.y -= Laplas->hitbox.y - window->h / 2.f;
			if (Laplas->hitbox.y > levelHeight - window->h / 2.f)
				rect123.y -= levelHeight - window->h;


			if (items[i].cost>0)
			{
				SDL_Rect rectCost = rect123;

				sprintf_s(cost_text, "%d", items[i].cost);
				surface = TTF_RenderText_Blended(font, cost_text, { 255, 199, 60, 255 });

				texture_cost.texture = SDL_CreateTextureFromSurface(ren, surface);
				texture_cost.textureSize.w = surface->w;
				texture_cost.textureSize.h = surface->h;
				texture_cost.textureSize.x = WINDOW_WIDTH - surface->w;
				texture_cost.textureSize.y = NULL;

				rectCost.y -= texture_cost.textureSize.h;

				SDL_RenderCopy(ren, texture_cost.texture, NULL, &rectCost);
				SDL_FreeSurface(surface);
				SDL_DestroyTexture(texture_cost.texture);

			}

			SDL_RenderCopy(ren, items[i].render.texture, &items[i].render.frame, &rect123);
			if ((SDL_GetTicks() - items[i].render.frameTime > 1000. / 30))
			{
				items[i].render.frame.x += items[i].render.textureSize.w / items[i].render.frameCount;
				items[i].render.frameTime = SDL_GetTicks();
			}

			if (items[i].render.frame.x >= items[i].render.textureSize.w)
				items[i].render.frame.x = 0;
		}


	}
}

void DrawTraps(int* trapsCount, mainTraps levelTraps[], mainHero* Laplas, mainWindow* window, SDL_Renderer* ren, int levelWidth, int levelHeight)
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

		if (levelTraps[i].type == 1 || levelTraps[i].type == 2)
		{
			if (levelTraps[i].gazeDirection == 1)
				SDL_RenderCopyEx(ren, levelTraps[i].render.texture, NULL, &movedTrap, 0, 0, SDL_FLIP_HORIZONTAL);
			else if (levelTraps[i].gazeDirection == 0)
				SDL_RenderCopyEx(ren, levelTraps[i].render.texture, NULL, &movedTrap, 0, 0, SDL_FLIP_NONE);
		}

		else if (levelTraps[i].type == 3)
		{
			SDL_Rect dopRect1 = levelTraps[i].render.textureSize;
			SDL_Rect dopRect2 = movedTrap;
			dopRect2.w = dopRect1.w;

			while (true)
			{
				if (dopRect2.x + dopRect2.w >= movedTrap.x + movedTrap.w)
				{
					dopRect2.w = dopRect1.w = movedTrap.x + movedTrap.w -  dopRect2.x;
					SDL_RenderCopyEx(ren, levelTraps[i].render.texture, &dopRect1, &dopRect2, 0, 0, SDL_FLIP_NONE);
					break;
				}
				SDL_RenderCopyEx(ren, levelTraps[i].render.texture, &dopRect1, &dopRect2, 0, 0, SDL_FLIP_NONE);
				dopRect2.x += dopRect2.w;
			}
		}

	}
}

void DrawHeroBullet(mainHero* Laplas, mainWindow* window, SDL_Renderer* ren, int levelWidth, int levelHeight)
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

			if (Laplas->battle.shoot[i].bulletSpeed > 0)
			{
				if (!Laplas->buffs.itemRubberBulletActive)
				{
					SDL_RenderCopyEx(ren, Laplas->animation.bullet.texture, NULL, &rect123, 0, 0, SDL_FLIP_NONE);
				}
				else
					SDL_RenderCopyEx(ren, Laplas->animation.rubber_bullet.texture, NULL, &rect123, 0, 0, SDL_FLIP_NONE);
			}
			else
			{
				if (!Laplas->buffs.itemRubberBulletActive)
				{
					SDL_RenderCopyEx(ren, Laplas->animation.bullet.texture, NULL, &rect123, 0, 0, SDL_FLIP_HORIZONTAL);
				}
				else
					SDL_RenderCopyEx(ren, Laplas->animation.rubber_bullet.texture, NULL, &rect123, 0, 0, SDL_FLIP_HORIZONTAL);
			}
		}
}

void DrawTrapsBullet(mainHero* Laplas, mainWindow* window, int* trapsCount, mainTraps levelTraps[], mainRenderer* trap_dart
	, SDL_Renderer* ren, int levelWidth, int levelHeight)
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

			if (levelTraps[i].shoot.bulletSpeed > 0)
				SDL_RenderCopyEx(ren, trap_dart->texture, NULL, &rect123, 0, 0, SDL_FLIP_NONE);
			else if (levelTraps[i].shoot.bulletSpeed < 0)
				SDL_RenderCopyEx(ren, trap_dart->texture, NULL, &rect123, 0, 0, SDL_FLIP_HORIZONTAL);
		}
}

void DrawLifeBar(mainHero laplas, mainRenderer hpBarTexture, mainRenderer hpBarEdgingTexture, mainWindow* window, SDL_Renderer* ren) {
	SDL_FRect hpBar = { float(window->w / 11), float(window->h) - float(window->h / 4.95), float(window->w / 32.8) * laplas.status.HP / 10,  float(window->h / 14.3) };
	SDL_FRect hpBarEdging = { float(window->w / 30), float(window->h) - float(window->h / 4.15), float(window->w / 2.7),  float(window->h / 7.2) };
	SDL_RenderCopyF(ren, hpBarTexture.texture, NULL, &hpBar);
	SDL_RenderCopyF(ren, hpBarEdgingTexture.texture, NULL, &hpBarEdging);
}

void DrawAmmoBar(mainRenderer ammoBarTexture, mainWindow* window, SDL_Renderer* ren) {
	SDL_FRect ammoBar = { float(window->w / 25.8), float(window->h) - float(window->h / 7.4), float(window->w / 6),  float(window->h / 11.07) };
	SDL_RenderCopyF(ren, ammoBarTexture.texture, NULL, &ammoBar);
}

void DrawEnemyHP(mainHero Laplas, mainEnemys* levelEnemys, int enemysCount, mainRenderer hpBarTexture, mainRenderer enemyHpBarEdgingTexture, mainWindow* window, SDL_Renderer* ren) {
	mainEnemys closerEnemy = levelEnemys[0];
	int minDistance = sqrt((Laplas.hitbox.x - levelEnemys[0].hitbox.x) * (Laplas.hitbox.x - levelEnemys[0].hitbox.x) +
		(Laplas.hitbox.y - levelEnemys[0].hitbox.y) * (Laplas.hitbox.y - levelEnemys[0].hitbox.y));
	int curDistance;
	for (int i = 0; i < enemysCount; i++)
	{
		curDistance = sqrt((Laplas.hitbox.x - levelEnemys[i].hitbox.x) * (Laplas.hitbox.x - levelEnemys[i].hitbox.x) +
			(Laplas.hitbox.y - levelEnemys[i].hitbox.y) * (Laplas.hitbox.y - levelEnemys[i].hitbox.y));
		if (minDistance > curDistance)
		{
			minDistance = curDistance;
			closerEnemy = levelEnemys[i];
		}
	}

	SDL_FRect hpBar = { float(window->w / 3.55), float(window->h / 16), float(window->w / 42.8) * closerEnemy.status.HP / 10,  float(window->h / 18) };
	SDL_FRect hpBarEdging = { float(window->w / 3.6), float(window->h / 16), float(window->w / 41.8) * closerEnemy.status.startHP/10,  float(window->h / 18) };
	SDL_RenderCopyF(ren, hpBarTexture.texture, NULL, &hpBar);
	SDL_RenderCopyF(ren, enemyHpBarEdgingTexture.texture, NULL, &hpBarEdging);
}

void DrawMoney(SDL_Renderer* ren, mainWindow* window, TTF_Font* font, mainHero Laplas) {
	SDL_Surface* surface = NULL;
	mainRenderer texture_money;
	char money_text[10] = "hobo";

	SDL_Rect rectCost = { float(window->w / 3.8), float(window->h) - float(window->h / 8.8), float(window->w / 12),  float(window->h / 16) };

	if (Laplas.money > 0)
	{
		sprintf_s(money_text, "%d", Laplas.money);
	}
	
	surface = TTF_RenderText_Blended(font, money_text, { 255, 199, 60, 255 });

	texture_money.texture = SDL_CreateTextureFromSurface(ren, surface);
	texture_money.textureSize.w = surface->w;
	texture_money.textureSize.h = surface->h;
	texture_money.textureSize.x = WINDOW_WIDTH - surface->w;
	texture_money.textureSize.y = NULL;

	SDL_RenderCopy(ren, texture_money.texture, NULL, &rectCost);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture_money.texture);
}