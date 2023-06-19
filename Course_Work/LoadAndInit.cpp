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
	Laplas.status = { HERO_DAMAGE, HERO_HP, ALIVE , HERO_START_AMUNITION, HERO_SHOOT_DAMAGE };
	Laplas.animationType = NULL;
	Laplas.buffs = { NULL, NULL, DMG_BUFF_DUARATION, DMG_BUFF_PERCENT, NULL };
	Laplas.keys = { NULL, NULL, NULL };


	return Laplas;
}

void InitEnemys(mainEnemys levelEnemys[], int* enemysCount, mainRenderer* texture_beaver_run, mainRenderer* texture_beaver_atack, 
	mainRenderer* texture_beaver_preAtack, mainRenderer* texture_krab_run)
{
	for (int i = 0; i < *enemysCount; i++)
	{
		if (levelEnemys[i].type == 1)
		{
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, BEAVER_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status = { BEAVER_DMG, BEAVER_HP , ALIVE };
			levelEnemys[i].effect.atackCD = BEAVER_ATACK_CD;
			levelEnemys[i].animation.run = *texture_beaver_run;
			levelEnemys[i].animation.atack = *texture_beaver_atack;
			levelEnemys[i].animation.preAtack = *texture_beaver_preAtack;
			levelEnemys[i].triggered = NULL;
			levelEnemys[i].triggeredDistance = BEAVER_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;

		}

		else if (levelEnemys[i].type == 2)
		{
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, BEAVER_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status = { BEAVER_DMG, BEAVER_HP , ALIVE };
			levelEnemys[i].effect.atackCD = BEAVER_ATACK_CD;
			levelEnemys[i].animation.run = *texture_beaver_run;
			levelEnemys[i].animation.atack = *texture_beaver_atack;
			levelEnemys[i].animation.preAtack = *texture_beaver_preAtack;
			levelEnemys[i].triggered = NULL;
			levelEnemys[i].triggeredDistance = BEAVER_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;


		}

		else if (levelEnemys[i].type == 3)
		{
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, KRAB_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status = { KRAB_DMG, KRAB_HP , ALIVE };
			levelEnemys[i].effect.atackCD = KRAB_ATACK_CD;
			levelEnemys[i].animation.run = *texture_krab_run;
			levelEnemys[i].animation.atack = *texture_krab_run;
			levelEnemys[i].animation.preAtack = *texture_krab_run;
			levelEnemys[i].triggered = NULL;
			levelEnemys[i].triggeredDistance = KRAB_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;

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

void InitItems(mainItems item[])
{
	for (int i = 0; i < ITEM_COUNT; i++)
	{
		item[i].hitbox = { NULL, NULL, NULL, NULL };
		item[i].alive = NULL;
		item[i].position = { NULL, NULL };
		item[i].render.frame = { NULL, NULL, NULL, NULL };
		item[i].render.frameCount = NULL;
		item[i].render.frameTime = NULL;
		item[i].render.texture = NULL;
		item[i].render.textureSize = { NULL, NULL, NULL, NULL };
		item[i].grab_zone = { NULL, NULL, NULL, NULL };
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


	fscanf_s(f, "%d ", &Laplas->buffs.DMG_buff_active);
	fscanf_s(f, "%d ", &Laplas->buffs.startDMG_buff);
	fscanf_s(f, "%d ", &Laplas->buffs.DMG_buffDuaration);
	fscanf_s(f, "%d ", &Laplas->buffs.DMG_buffPercent);

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