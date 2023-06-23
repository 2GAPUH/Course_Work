#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "common_parameters.h"
#include <time.h>

mainBorders* LoadLevel(mainBorders* levelBorders, int* bordersCount, mainHero* Laplas, const char levelName[], int *levelWidth, 
	int* levelHeight)
{
	FILE* f;
	fopen_s(&f, levelName, "r");

	fscanf_s(f, "%d", bordersCount);
	fscanf_s(f, "%d", &Laplas->hitbox.x);
	fscanf_s(f, "%d", &Laplas->hitbox.y);
	fscanf_s(f, "%d", levelWidth);
	fscanf_s(f, "%d", levelHeight);

	levelBorders = (mainBorders*)realloc(levelBorders, sizeof(mainBorders) * (*bordersCount));



	for (int i = 0; i < *bordersCount; i++)
	{
		fscanf_s(f, "%d", &levelBorders[i].type);
		fscanf_s(f, "%d", &levelBorders[i].bordersHitbox.x);
		fscanf_s(f, "%d", &levelBorders[i].bordersHitbox.y);
		fscanf_s(f, "%d", &levelBorders[i].bordersHitbox.w);
		fscanf_s(f, "%d", &levelBorders[i].bordersHitbox.h);
		levelBorders[i].alive = 1;
		levelBorders[i].active = 0;
		levelBorders[i].timer = 0;
		
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

	for (int i = 0; i < *enemysCount; i++)
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

mainItems* LoadItems(mainItems* levelItems, int* itemsCount, const char levelName[])
{
	FILE* f;
	fopen_s(&f, levelName, "r");

	fscanf_s(f, "%d", itemsCount);

	levelItems = (mainItems*)realloc(levelItems, sizeof(mainItems) * (*itemsCount));

	for (int i = 0; i < *itemsCount; i++)
	{
		fscanf_s(f, "%d", &levelItems[i].type);

		fscanf_s(f, "%d", &levelItems[i].hitbox.x);
		fscanf_s(f, "%d", &levelItems[i].hitbox.y);
		fscanf_s(f, "%d", &levelItems[i].hitbox.w);
		fscanf_s(f, "%d", &levelItems[i].hitbox.h);

		fscanf_s(f, "%d", &levelItems[i].alive);
		if (GetNumInRange(0, 10) < levelItems[i].alive)
			levelItems[i].alive = 1;
		else
			levelItems[i].alive = 0;
	}

	fclose(f);

	return levelItems;
}

mainHero InitHero()
{
	mainHero Laplas;
	Laplas.position = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
	Laplas.hitbox = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, HERO_WIDHT, HERO_HEIGHT };
	Laplas.texture_rect = { NULL, NULL, NULL, NULL };
	Laplas.physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, HERO_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
	Laplas.effect = { HERO_DASH_CD, NULL, HERO_ATACK_CD, NULL, HERO_SHOOT_CD, NULL, CAMERA_SCALE_X, CAMERA_SCALE_Y, NULL, HERO_AFTER_ATACK_PROTECTION, NULL };
	Laplas.animation.com = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas.animation.run = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas.animation.punch = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas.battle = { NULL, { NULL, NULL } , NULL, NULL };
	Laplas.status = { HERO_HP, HERO_DAMAGE, HERO_HP, ALIVE , HERO_START_AMUNITION, HERO_SHOOT_DAMAGE };
	Laplas.animationType = NULL;
	Laplas.buffs = { NULL, NULL, DMG_BUFF_DUARATION, DMG_BUFF_PERCENT, NULL };
	Laplas.keys = { NULL, NULL, NULL };


	return Laplas;
}

void InitEnemys(mainEnemys levelEnemys[], int* enemysCount, mainRenderer* texture_beaver_run, mainRenderer* texture_beaver_atack, 
	mainRenderer* texture_beaver_preAtack, mainRenderer* texture_krab_run, mainRenderer* texture_acid_effect, mainRenderer* texture_tower, mainRenderer* texture_tower_bullet)
{
	for (int i = 0; i < *enemysCount; i++)
	{
		levelEnemys[i].animation.acid_effect = *texture_acid_effect;
		levelEnemys[i].effect.poisoned = 0;
		levelEnemys[i].effect.poisonLastDamage = 0;
		levelEnemys[i].triggered = NULL;

		if (levelEnemys[i].type == 1)
		{
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, BEAVER_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status = { BEAVER_HP, BEAVER_DMG, BEAVER_HP , ALIVE };
			levelEnemys[i].effect.atackCD = BEAVER_ATACK_CD;
			levelEnemys[i].animation.run = *texture_beaver_run;
			levelEnemys[i].animation.atack = *texture_beaver_atack;
			levelEnemys[i].animation.preAtack = *texture_beaver_preAtack;
			levelEnemys[i].triggeredDistance = BEAVER_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;


		}

		else if (levelEnemys[i].type == 2)
		{
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, BEAVER_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status = { BEAVER_HP, BEAVER_DMG, BEAVER_HP , ALIVE };
			levelEnemys[i].effect.atackCD = BEAVER_ATACK_CD;
			levelEnemys[i].animation.run = *texture_beaver_run;
			levelEnemys[i].animation.atack = *texture_beaver_atack;
			levelEnemys[i].animation.preAtack = *texture_beaver_preAtack;
			levelEnemys[i].triggeredDistance = BEAVER_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;


		}

		else if (levelEnemys[i].type == 3)
		{
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, KRAB_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status = { KRAB_HP, KRAB_DMG, KRAB_HP , ALIVE };
			levelEnemys[i].effect.atackCD = KRAB_ATACK_CD;
			levelEnemys[i].animation.run = *texture_krab_run;
			levelEnemys[i].animation.atack = *texture_krab_run;
			levelEnemys[i].animation.preAtack = *texture_krab_run;
			levelEnemys[i].triggeredDistance = KRAB_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;

		}

		else if (levelEnemys[i].type == 4)
		{
			{
				levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, KRAB_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
				levelEnemys[i].effect.underAtack = NULL;
				levelEnemys[i].status = { KRAB_HP, KRAB_DMG, KRAB_HP , ALIVE };
				levelEnemys[i].effect.atackCD = KRAB_ATACK_CD;
				levelEnemys[i].animation.run = *texture_tower;
				levelEnemys[i].animation.atack = *texture_tower;
				levelEnemys[i].animation.preAtack = *texture_tower;
				levelEnemys[i].triggeredDistance = KRAB_TRIGGERED_DISTANCE;
				levelEnemys[i].animation_type = NULL;
				levelEnemys[i].animation.bullet = *texture_tower_bullet;
				levelEnemys[i].shoot.alive = 0;

			}
		}
	}
}

void InitTraps(mainTraps levelTraps[], int* trapsCount, mainRenderer* texture_dart_trap, mainRenderer* texture_pressure_plate, 
	mainRenderer* texture_trap_spikes)
{
	for (int i = 0; i < *trapsCount; i++)
	{
		if (levelTraps[i].type == 1)
		{
			levelTraps[i].render = *texture_dart_trap;
			levelTraps[i].shoot.alive = NULL;
			levelTraps[i].shoot.bulletSpeed = TRAPS_BULLET_SPEED;
			levelTraps[i].shoot.shootAtackCentere = { NULL, NULL };
			levelTraps[i].triggered = NULL;
			levelTraps[i].DMG = TRAP_DART_DMG;
			levelTraps[i].cooldown = TRAPS_DART_CD;
			levelTraps[i].lastShoot = NULL;

		}

		else if (levelTraps[i].type == 2)
		{
			levelTraps[i].render = *texture_pressure_plate;
			levelTraps[i].shoot.alive = NULL;
			levelTraps[i].shoot.bulletSpeed = TRAPS_BULLET_SPEED;
			levelTraps[i].shoot.shootAtackCentere = { NULL, NULL };
			levelTraps[i].triggered = NULL;
			levelTraps[i].DMG = NULL;
			levelTraps[i].cooldown = NULL;
			levelTraps[i].lastShoot = NULL;
		}

		else if (levelTraps[i].type == 3)
		{
			levelTraps[i].render = *texture_trap_spikes;
			levelTraps[i].shoot.alive = NULL;
			levelTraps[i].shoot.bulletSpeed = HERO_BULLET_SPEED;
			levelTraps[i].shoot.shootAtackCentere = { NULL, NULL };
			levelTraps[i].triggered = NULL;
			levelTraps[i].DMG = TRAP_SPIKES_DMG;
			levelTraps[i].cooldown = NULL;
			levelTraps[i].lastShoot = NULL;
		}
	}
}

void InitItems(mainItems levelItems[], int* itemsCount, mainRenderer* texture_buff_DMG, mainRenderer* texture_item_Rubber_Bullet, 
	mainRenderer* texture_barrel, mainRenderer* texture_item_Ball, mainRenderer* texture_item_acid, mainRenderer* texture_buff_speed, 
	mainRenderer*  texture_buff_lucky, mainRenderer* texture_skill_figure)
{
	for (int i = 0; i < *itemsCount; i++)
	{
		if (levelItems[i].alive)
		{
			levelItems[i].position = { levelItems[i].hitbox.x + levelItems[i].hitbox.w/2, levelItems[i].hitbox.y + levelItems[i].hitbox.h / 2 };
			levelItems[i].grab_zone = { levelItems[i].hitbox.x - levelItems[i].hitbox.w, levelItems[i].hitbox.y - levelItems[i].hitbox.h, levelItems[i].hitbox.w * 2, levelItems[i].hitbox.h * 2 };

			int l;
			

			switch (levelItems[i].type)
			{
			case 1:
				levelItems[i].render = *texture_barrel;
				break;

			case 2:
				l = GetNumInRange(1, 3);
				switch (l)
				{
				case 1:
					levelItems[i].render = *texture_buff_DMG;
					levelItems[i].dop_type = 1;
					break;

				case 2:
					levelItems[i].render = *texture_buff_speed;
					levelItems[i].dop_type = 2;
					break;

				case 3:
					levelItems[i].render = *texture_buff_lucky;
					levelItems[i].dop_type = 3;
					break;
				}


				break;

			case 3:
				l = GetNumInRange(1, 3);
				switch (l)
				{
				case 1:
					levelItems[i].render = *texture_item_Ball;
					levelItems[i].dop_type = 1;
					levelItems[i].cost = 100;
					break;

				case 2:
					levelItems[i].render = *texture_item_Rubber_Bullet;
					levelItems[i].dop_type = 2;
					levelItems[i].cost = 300;
					break;

				case 3:
					levelItems[i].render = *texture_item_acid;
					levelItems[i].dop_type = 3;
					levelItems[i].cost = 200;
					break;
				}
				
				break;

			case 5:
				levelItems[i].render = *texture_skill_figure;
				break;

			}
			
		}
	}
}

void loadLaplas(mainHero* Laplas, const char path[], int* deltatime)
{
	FILE* f;
	if (fopen_s(&f, path, "r") != 0)
	{
		printf_s("Can't open %s!", path);
		system("pause");
	}

	fscanf_s(f, "%d ", &deltatime);

	fscanf_s(f, "%d ", &Laplas->position.x);
	fscanf_s(f, "%d ", &Laplas->position.y);

	fscanf_s(f, "%d ", &Laplas->hitbox.x);
	fscanf_s(f, "%d ", &Laplas->hitbox.y);
	fscanf_s(f, "%d ", &Laplas->hitbox.w);
	fscanf_s(f, "%d ", &Laplas->hitbox.h);

	fscanf_s(f, "%d ", &Laplas->texture_rect.x);
	fscanf_s(f, "%d ", &Laplas->texture_rect.y);
	fscanf_s(f, "%d ", &Laplas->texture_rect.w);
	fscanf_s(f, "%d ", &Laplas->texture_rect.h);

	fscanf_s(f, "%f ", &Laplas->physic.xMoveL);
	fscanf_s(f, "%f ", &Laplas->physic.xMoveR);
	fscanf_s(f, "%f ", &Laplas->physic.yMove);
	fscanf_s(f, "%d ", &Laplas->physic.gazeDirection);
	fscanf_s(f, "%f ", &Laplas->physic.speed);
	fscanf_s(f, "%f ", &Laplas->physic.gravity);
	fscanf_s(f, "%f ", &Laplas->physic.accelerationX);
	fscanf_s(f, "%f ", &Laplas->physic.accelerationY);
	fscanf_s(f, "%f ", &Laplas->physic.impulse);
	fscanf_s(f, "%d ", &Laplas->physic.onBorder);

	fscanf_s(f, "%d ", &Laplas->effect.dashCD);
	fscanf_s(f, "%d ", &Laplas->effect.timeDashCD);
	fscanf_s(f, "%d ", &Laplas->effect.atackCD);
	fscanf_s(f, "%d ", &Laplas->effect.timeAtackCD);
	fscanf_s(f, "%d ", &Laplas->effect.shootCD);
	fscanf_s(f, "%d ", &Laplas->effect.timeShootCD);
	fscanf_s(f, "%f ", &Laplas->effect.camersScale.x);
	fscanf_s(f, "%f ", &Laplas->effect.camersScale.y);
	fscanf_s(f, "%d ", &Laplas->effect.underAtack);
	fscanf_s(f, "%d ", &Laplas->effect.afterAtackResist);
	fscanf_s(f, "%d ", &Laplas->effect.lastDamage);

	fscanf_s(f, "%d ", &Laplas->status.DMG);
	fscanf_s(f, "%d ", &Laplas->status.HP);
	fscanf_s(f, "%d ", &Laplas->status.alive);
	fscanf_s(f, "%d ", &Laplas->status.ammunition);
	fscanf_s(f, "%d ", &Laplas->status.Shoot_DMG);


	fscanf_s(f, "%d ", &Laplas->buffs.buffDMGactive);
	fscanf_s(f, "%d ", &Laplas->buffs.buffDMGstart);
	fscanf_s(f, "%d ", &Laplas->buffs.buffDuaration);
	fscanf_s(f, "%d ", &Laplas->buffs.buffDMGpercent);

	fclose(f);
}

void dopLoadHero(mainHero* Laplas)
{
	Laplas->animation.com = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas->animation.run = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas->animation.punch = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas->battle = { NULL, { NULL, NULL } , NULL, NULL };
	Laplas->animationType = NULL;
	Laplas->keys = { NULL, NULL, NULL };

}