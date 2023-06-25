#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include "common_parameters.h"
#include <time.h>

mainBorders* LoadLevel(mainBorders* levelBorders, int* bordersCount, mainHero* Laplas, const char levelName[], int *levelWidth, 
	int* levelHeight)
{
	FILE* f;
	if (fopen_s(&f, levelName, "r") != 0)
	{
		puts(levelName);
		printf_s("Can't open!\n");
		system("pause");
	}

	fscanf_s(f, "%d", bordersCount);
	fscanf_s(f, "%d", levelWidth);
	fscanf_s(f, "%d", levelHeight);

	levelBorders = (mainBorders*)malloc(sizeof(mainBorders) * (*bordersCount));



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
	

	if (fopen_s(&f, levelName, "r") != 0)
	{
		puts(levelName);
		printf_s("Can't open!\n");
		system("pause");
	}


	fscanf_s(f, "%d", enemysCount);


	levelEnemys = (mainEnemys*)malloc(sizeof(mainEnemys) * (*enemysCount));

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
	

	if (fopen_s(&f, levelName, "r") != 0)
	{
		puts(levelName);
		printf_s("Can't open!\n");
		system("pause");
	}

	fscanf_s(f, "%d", trapsCount);

	levelTraps = (mainTraps*)malloc(sizeof(mainTraps) * (*trapsCount));

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

	if (fopen_s(&f, levelName, "r") != 0)
	{
		puts(levelName);
		printf_s("Can't open!\n");
		system("pause");
	}

	fscanf_s(f, "%d", itemsCount);

	levelItems = (mainItems*)malloc(sizeof(mainItems) * (*itemsCount));

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

void InitHero(mainHero* Laplas)
{
	Laplas->texture_rect = { NULL, NULL, NULL, NULL };
	Laplas->hitbox = { WINDOW_WIDTH/2, WINDOW_HEIGHT/2, HERO_WIDHT, HERO_HEIGHT };
	Laplas->position = { 0, 0 };
	Laplas->physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, HERO_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
	Laplas->effect = { HERO_DASH_CD, NULL, HERO_ATACK_CD, NULL, HERO_SHOOT_CD, NULL, CAMERA_SCALE_X, CAMERA_SCALE_Y, NULL, HERO_AFTER_ATACK_PROTECTION, NULL };
	Laplas->animation.com = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas->animation.run = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas->animation.punch = { NULL, {0, 0, 0, 0} , {0, 0, 0 ,0}, NULL };
	Laplas->battle = { NULL, { NULL, NULL } , NULL, NULL };
	Laplas->status.HP = int(HERO_HP * (Laplas->status.lvlHealth / 5. + 1));
	Laplas->status.DMG = int(HERO_DAMAGE * (Laplas->status.lvlHealth / 10. + 1));
	Laplas->status.startHP = (HERO_HP * (Laplas->status.lvlHealth / 5. + 1));
	Laplas->status.alive = ALIVE;
	Laplas->status.lvlWeapon = int(HERO_START_AMUNITION * (Laplas->status.lvlHealth / 5. + 1));
	Laplas->status.Shoot_DMG = int(HERO_SHOOT_DAMAGE * (Laplas->status.lvlHealth / 15. + 1));
	Laplas->status.lvlLucky = Laplas->status.lvlLucky * 3;
	Laplas->status.potion_duar = Laplas->status.potion_duar * 10;

	Laplas->animationType = 1;
	Laplas->buffs = { NULL, NULL, DMG_BUFF_DUARATION, DMG_BUFF_PERCENT, NULL };
	Laplas->keys = { NULL, NULL, NULL };
	Laplas->lastLocalSwap = 0;

}

void dopInitHero(mainHero* Laplas)
{
	Laplas->status.HP = int(HERO_HP * (Laplas->status.lvlHealth / 5. + 1));
	Laplas->status.DMG = int(HERO_DAMAGE * (Laplas->status.lvlHealth / 10. + 1));
	Laplas->status.startHP = (HERO_HP * (Laplas->status.lvlHealth / 5. + 1));
	Laplas->status.Shoot_DMG = int(HERO_SHOOT_DAMAGE * (Laplas->status.lvlHealth / 15. + 1));
	Laplas->status.lucky = Laplas->status.lvlLucky * 3;
	Laplas->status.potion_duar = Laplas->status.lvlPotion * 10;
	Laplas->status.ammunition = int(HERO_START_AMUNITION * (Laplas->status.lvlAmmo / 8. + 1));
}

void InitEnemys(mainEnemys levelEnemys[], int* enemysCount, mainRenderer* texture_beaver_run, mainRenderer* texture_beaver_atack,
	mainRenderer* texture_beaver_preAtack, mainRenderer* texture_krab_run, mainRenderer* texture_acid_effect,
	mainRenderer* texture_tower, mainRenderer* texture_tower_bullet, mainRenderer* texture_digit_idle, mainRenderer* texture_digit_atack,
	mainRenderer* texture_bat_with_anvil, mainRenderer* texture_bat_without_anvil, mainRenderer* texture_anvil_without_bat, 
	mainRenderer* texture_barrel)
{
	for (int i = 0; i < *enemysCount; i++)
	{
		levelEnemys[i].animation.acid_effect = *texture_acid_effect;
		levelEnemys[i].effect.poisoned = 0;
		levelEnemys[i].effect.poisonLastDamage = 0;
		levelEnemys[i].triggered = NULL;

		switch(levelEnemys[i].type)
		{	
		case 1:
			//BEAVER
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, BEAVER_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status.HP = BEAVER_HP;
			levelEnemys[i].status.DMG = BEAVER_DMG;
			levelEnemys[i].status.startHP = BEAVER_HP;
			levelEnemys[i].status.alive = true;
			levelEnemys[i].effect.atackCD = BEAVER_ATACK_CD;
			levelEnemys[i].animation.run = *texture_beaver_run;
			levelEnemys[i].animation.atack = *texture_beaver_atack;
			levelEnemys[i].animation.preAtack = *texture_beaver_preAtack;
			levelEnemys[i].triggeredDistance = BEAVER_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;
			levelEnemys[i].reward = BEAVER_REAWRD;
			break;
		
		case 2:
			//BARREL
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, BARREL_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status.HP = BARREL_HP;
			levelEnemys[i].status.DMG = BARREL_DMG;
			levelEnemys[i].status.startHP = BARREL_HP;
			levelEnemys[i].status.alive = true;
			levelEnemys[i].effect.atackCD = BARREL_ATACK_CD;
			levelEnemys[i].animation.run = *texture_barrel;
			levelEnemys[i].animation.atack = *texture_barrel;
			levelEnemys[i].animation.preAtack = *texture_barrel;
			levelEnemys[i].triggeredDistance = BARREL_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;
			levelEnemys[i].reward = BARREL_REAWRD;
			break;

		case 3:
			//KRAB
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, KRAB_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status.HP = KRAB_HP;
			levelEnemys[i].status.DMG = KRAB_DMG;
			levelEnemys[i].status.startHP = KRAB_HP;
			levelEnemys[i].status.alive = true;
			levelEnemys[i].effect.atackCD = KRAB_ATACK_CD;
			levelEnemys[i].animation.run = *texture_krab_run;
			levelEnemys[i].animation.atack = *texture_krab_run;
			levelEnemys[i].animation.preAtack = *texture_krab_run;
			levelEnemys[i].triggeredDistance = KRAB_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;
			levelEnemys[i].reward = KRAB_REAWRD;
			break;
		

		case 4:
			//TOWER
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, KRAB_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status.HP = TOWER_HP;
			levelEnemys[i].status.DMG = TOWER_DMG;
			levelEnemys[i].status.startHP = TOWER_HP;
			levelEnemys[i].status.alive = true;
			levelEnemys[i].effect.atackCD = TOWER_ATACK_CD;
			levelEnemys[i].animation.run = *texture_tower;
			levelEnemys[i].animation.atack = *texture_tower;
			levelEnemys[i].animation.preAtack = *texture_tower;
			levelEnemys[i].triggeredDistance = TOWER_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;
			levelEnemys[i].animation.bullet = *texture_tower_bullet;
			levelEnemys[i].shoot.alive = 0;
			levelEnemys[i].reward = BEAVER_REAWRD;
			break;
		

		case 5:
			//DIGLET
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, DIGIT_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status.HP = DIGIT_HP;
			levelEnemys[i].status.DMG = DIGIT_DMG;
			levelEnemys[i].status.startHP = DIGIT_HP;
			levelEnemys[i].status.alive = true;
			levelEnemys[i].effect.atackCD = DIGIT_ATACK_CD;
			levelEnemys[i].animation.run = *texture_digit_idle;
			levelEnemys[i].animation.atack = *texture_digit_atack;
			levelEnemys[i].animation.preAtack = *texture_digit_idle;
			levelEnemys[i].triggeredDistance = DIGIT_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = NULL;
			levelEnemys[i].reward = DIGIT_REAWRD;
			break;

		case 6:
			//Bat
			levelEnemys[i].physic = { X_MOVE_L, X_MOVE_R, Y_MOVE, GAZE_DIRECTION, BAT_SPEED, GRAVITY, ACCELERATION_Y, ACCELERATION_X, IMPULSE, ON_BORDER };
			levelEnemys[i].effect.underAtack = NULL;
			levelEnemys[i].status.HP = BAT_HP;
			levelEnemys[i].status.DMG = BAT_DMG;
			levelEnemys[i].status.startHP = BAT_HP;
			levelEnemys[i].status.alive = true;
			levelEnemys[i].effect.atackCD = BAT_ATACK_CD;
			levelEnemys[i].animation.run = *texture_bat_with_anvil;
			levelEnemys[i].animation.atack = *texture_bat_without_anvil;
			levelEnemys[i].animation.preAtack = *texture_bat_with_anvil;
			levelEnemys[i].triggeredDistance = BAT_TRIGGERED_DISTANCE;
			levelEnemys[i].animation_type = 1;
			levelEnemys[i].animation.bullet = *texture_anvil_without_bat;
			levelEnemys[i].shoot.bulletSpeed = TRAPS_BULLET_SPEED;
			levelEnemys[i].shoot.alive = 0;
			levelEnemys[i].reward = BAT_REAWRD;
			levelEnemys[i].shoot.everShoot = 0;
			break;



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
	mainRenderer*  texture_buff_lucky, mainRenderer* texture_skill_figure, mainRenderer* texture_kebab, mainRenderer* texture_shop)
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
					levelItems[i].cost = 0;
					break;

				case 2:
					levelItems[i].render = *texture_buff_speed;
					levelItems[i].dop_type = 2;
					levelItems[i].cost = 0;
					break;

				case 3:
					levelItems[i].render = *texture_buff_lucky;
					levelItems[i].dop_type = 3;
					levelItems[i].cost = 0;
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

			case 4:
				levelItems[i].render = *texture_kebab;
				levelItems[i].cost = 0;
				break;

			case 5:
				levelItems[i].render = *texture_skill_figure;
				levelItems[i].cost = 0;
				levelItems[i].alive = 1;
				break;

			case 6:
				levelItems[i].render = *texture_shop;
				levelItems[i].cost = 0;
				break;
			}
			
		}
	}
}

void loadLaplas(mainHero* Laplas, const char path[], Settings* settings)
{
	FILE* f;
	if (fopen_s(&f, path, "r") != 0)
	{
		printf_s("Can't open %s!", path);
		system("pause");
	}

	fscanf_s(f, "%d ", &Laplas->status.lvlAmmo);
	fscanf_s(f, "%d ", &Laplas->status.lvlHealth);
	fscanf_s(f, "%d ", &Laplas->status.lvlLucky);
	fscanf_s(f, "%d ", &Laplas->status.lvlPotion);
	fscanf_s(f, "%d ", &Laplas->status.lvlStrength);
	fscanf_s(f, "%d ", &Laplas->status.lvlWeapon);

	fscanf_s(f, "%d ", &Laplas->status.souls);

	fscanf_s(f, "%d ", &settings->skin);
	
	fclose(f);
}



void saveLaplas(mainHero* Laplas, const char path[], Settings* settings)
{
	FILE* f;
	if (fopen_s(&f, path, "w") != 0)
	{
		printf_s("Can't open %s!", path);
		system("pause");
	}

	fprintf_s(f, "%d ", Laplas->status.lvlAmmo);
	fprintf_s(f, "%d ", Laplas->status.lvlHealth);
	fprintf_s(f, "%d ", Laplas->status.lvlLucky);
	fprintf_s(f, "%d ", Laplas->status.lvlPotion);
	fprintf_s(f, "%d ", Laplas->status.lvlStrength);
	fprintf_s(f, "%d ", Laplas->status.lvlWeapon);

	fprintf_s(f, "%d ", Laplas->status.souls);

	fprintf_s(f, "%d ", settings->skin);

	fclose(f);
}