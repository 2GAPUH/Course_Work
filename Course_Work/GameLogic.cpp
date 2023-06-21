#include "GameLogic.h"

bool HeroPhysicInRange(SDL_Point unit, SDL_Rect bordersHitbox)
{
	if (unit.x > bordersHitbox.x && unit.x < bordersHitbox.x + bordersHitbox.w)
		if (unit.y > bordersHitbox.y && unit.y < bordersHitbox.y + bordersHitbox.h)
			return 1;
	return 0;
}

void HeroBulletHitboxInRange(mainHero* Laplas, int* bordersCount, mainBorders levelBorders[])
{
	for (int f = 0; f < 10; f++)
	{
		if (Laplas->battle.shoot[f].alive)
		{
			for (int g = 0; g < *bordersCount; g++)
			{
				if (levelBorders[g].type != 6 && levelBorders[g].type != 5 && HeroPhysicInRange(Laplas->battle.shoot[f].shootAtackCentere, levelBorders[g].bordersHitbox))
				{
					if (Laplas->buffs.itemRubberBulletActive && Laplas->battle.shoot[f].rebound_count > 0)
					{
						Laplas->battle.shoot[f].bulletSpeed *= -1;
						Laplas->battle.shoot[f].rebound_count--;
						continue;
					}
					Laplas->battle.shoot[f].alive = 0;
				}
			}
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
				if (levelBorders[g].type != 6 && levelBorders[g].type != 5 && HeroPhysicInRange(levelTraps[f].shoot.shootAtackCentere, levelBorders[g].bordersHitbox))
					levelTraps[f].shoot.alive = 0;
		}
	}
}

void HeroBulletOutworldCheck(mainHero* Laplas, mainBorders levelBorders[])
{
	for (int i = 0; i < 10; i++)
		if (Laplas->battle.shoot[i].alive)
		{
			if (Laplas->battle.shoot[i].shootAtackCentere.x > levelBorders[1].bordersHitbox.w - HERO_BULLET_WIDTH / 2)
			{
				if (Laplas->buffs.itemRubberBulletActive && Laplas->battle.shoot[i].rebound_count > 0)
				{
					Laplas->battle.shoot[i].bulletSpeed *= -1;
					Laplas->battle.shoot[i].rebound_count--;
					continue;
				}
				Laplas->battle.shoot[i].alive = 0;
			}
			else if (Laplas->battle.shoot[i].shootAtackCentere.x < levelBorders[0].bordersHitbox.x + HERO_BULLET_WIDTH / 2)
			{
				if (Laplas->buffs.itemRubberBulletActive && Laplas->battle.shoot[i].rebound_count > 0)
				{
					Laplas->battle.shoot[i].bulletSpeed *= -1;
					Laplas->battle.shoot[i].rebound_count--;
					continue;
				}
				Laplas->battle.shoot[i].alive = 0;
			}


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

void EnemyTrigger(mainEnemys levelEnemys[], mainHero* Laplas, int* enemysCount)
{
	for (int i = 0; i < *enemysCount; i++)
		if (!levelEnemys[i].triggered && sqrt((Laplas->hitbox.x - levelEnemys[i].hitbox.x) * (Laplas->hitbox.x - levelEnemys[i].hitbox.x) +
			(Laplas->hitbox.y - levelEnemys[i].hitbox.y) * (Laplas->hitbox.y - levelEnemys[i].hitbox.y)) <
			levelEnemys[i].triggeredDistance || levelEnemys[i].effect.underAtack)
			levelEnemys[i].triggered = 1;
}

void EnemysMovement(int* enemysCount, mainEnemys levelEnemys[], mainHero* Laplas)
{
	for (int i = 0; i < *enemysCount; i++)
	{
		if (levelEnemys[i].triggered && levelEnemys[i].animation_type != 6)
		{
			if (levelEnemys[i].hitbox.x + levelEnemys[i].physic.speed < Laplas->hitbox.x)
			{
				levelEnemys[i].hitbox.x += levelEnemys[i].physic.speed;
				levelEnemys[i].physic.xMoveR = 1;
				levelEnemys[i].physic.xMoveL = 0;
			}
			else if (levelEnemys[i].hitbox.x - levelEnemys[i].physic.speed > Laplas->hitbox.x)
			{
				levelEnemys[i].hitbox.x -= levelEnemys[i].physic.speed;
				levelEnemys[i].physic.xMoveR = 0;
				levelEnemys[i].physic.xMoveL = -1;
			}
		}
	}
}


/*
2.1 DMG_Buff
2.2 Empty

3.1 Ball
3.2 Rubber bullet
*/
void ItemEquip(mainHero* Laplas, mainItems items[], int* itemsCount, int timeInGame)
{
	for (int i = 0; i < *itemsCount; i++)
		if (Laplas->keys.pressed_E && items[i].alive)
			if (HeroPhysicInRange({ Laplas->hitbox.x, Laplas->hitbox.y }, items[i].grab_zone))
			{
				switch (items[i].type)
				{
				case 2:
					switch (items[i].dop_type)
					{
					case 1:
						Laplas->buffs.buffDMGactive = 1;
						Laplas->buffs.buffDMGpercent = DMG_BUFF_PERCENT;
						Laplas->buffs.buffDuaration = DMG_BUFF_DUARATION;
						Laplas->buffs.buffDMGstart = timeInGame;
						Laplas->status.DMG = HERO_DAMAGE * DMG_BUFF_PERCENT;
						Laplas->status.Shoot_DMG = HERO_SHOOT_DAMAGE * DMG_BUFF_PERCENT;
						break;

					case 2:
						//Empty
						break;
					}
					break;

				case 3:
					switch (items[i].dop_type)
					{
					case 1:
						Laplas->buffs.itemBallActive = 1;
						break;

					case 2:
						Laplas->buffs.itemRubberBulletActive = 1;
						break;

					case 3:
						Laplas->buffs.itemAcid = 1;
					}
					break;
				}

				items[i].alive = 0;
			}
}

void BuffsStateCheck(mainHero* Laplas, int timeInGame)
{
	if (Laplas->buffs.buffDMGstart + Laplas->buffs.buffDuaration < timeInGame)
	{
		Laplas->buffs.buffDMGactive = 0;
		Laplas->status.Shoot_DMG = HERO_SHOOT_DAMAGE;
		Laplas->status.DMG = HERO_DAMAGE;
	}
}

void EnemyDeath(int* enemysCount, mainEnemys levelEnemys[])
{
	for (int d = 0; d < *enemysCount; d++)
		if (!levelEnemys[d].status.alive)
		{
			if (d == *enemysCount - 1 || *enemysCount == 1)
			{
				(*enemysCount)--;
			}

			mainEnemys tmpEnemy0 = levelEnemys[d];
			for (int e = *enemysCount - 1; e >= d; e--)
			{
				if (levelEnemys[e].status.alive)
				{
					levelEnemys[d] = levelEnemys[e];
					levelEnemys[e] = tmpEnemy0;
					(*enemysCount)--;
					break;
				}
				if (e == d)
				{
					(*enemysCount)--;
				}
			}
			break;
		}
}

void TrapActivate(int* trapsCount, mainTraps levelTraps[], mainHero* Laplas, int* deltaTime)
{
	for (int t = 0; t < *trapsCount; t++)
		if (levelTraps[t].type == 2 && levelTraps[t].triggered && !levelTraps[t - 1].shoot.alive && levelTraps[t - 1].cooldown + levelTraps[t - 1].lastShoot < *deltaTime)
		{
			levelTraps[t - 1].lastShoot = *deltaTime;
			if (levelTraps[t - 1].gazeDirection == 0)
			{
				levelTraps[t - 1].shoot.shootAtackCentere.x = levelTraps[t - 1].hitbox.x + levelTraps[t - 1].hitbox.w / 2;
				levelTraps[t - 1].shoot.shootAtackCentere.y = levelTraps[t - 1].hitbox.y + levelTraps[t - 1].hitbox.h / 2;
				levelTraps[t - 1].shoot.bulletSpeed = TRAPS_BULLET_SPEED;
				levelTraps[t - 1].shoot.alive = 1;
			}
			else
			{
				levelTraps[t - 1].shoot.shootAtackCentere.x = levelTraps[t - 1].hitbox.x - levelTraps[t - 1].hitbox.w / 2;
				levelTraps[t - 1].shoot.shootAtackCentere.y = levelTraps[t - 1].hitbox.y + levelTraps[t - 1].hitbox.h / 2;
				levelTraps[t - 1].shoot.bulletSpeed = -TRAPS_BULLET_SPEED;
				levelTraps[t - 1].shoot.alive = 1;
			}
		}
		else if (levelTraps[t].type == 3 && levelTraps[t].triggered && !Laplas->buffs.itemBallActive)
		{
			Laplas->status.HP -= levelTraps[t].DMG;
		}
}

void TrapAtack(int* trapsCount, mainTraps levelTraps[], mainHero* Laplas, int* deltaTime)
{
	for (int j = 0; j < *trapsCount; j++)
		if (levelTraps[j].shoot.alive)
		{
			levelTraps[j].shoot.shootAtackCentere.x += levelTraps[j].shoot.bulletSpeed;

			if (!Laplas->effect.underAtack && CheckShootHitbox(&levelTraps[j].shoot.shootAtackCentere, &Laplas->hitbox))
			{
				levelTraps[j].shoot.alive = 0;
				Laplas->effect.underAtack = 1;
				Laplas->effect.lastDamage = *deltaTime;
				if (!Laplas->buffs.itemBallActive)
				{
					Laplas->status.HP -= levelTraps[j].DMG;
				}
				else
					Laplas->buffs.itemBallActive = 0;

				if (Laplas->status.HP <= 0)
				{
					Laplas->status.alive = 0;
				}
			}

		}
}
