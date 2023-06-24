#pragma once
#include <SDL.h>
#include <SDL_Image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define FPS 60
#define HERO_WIDHT 50
#define HERO_HEIGHT 80
#define X_MOVE_L 0
#define X_MOVE_R 0
#define Y_MOVE 0
#define GAZE_DIRECTION 1
#define GRAVITY 10
#define ACCELERATION_Y 1
#define ACCELERATION_X 1
#define IMPULSE 0
#define ON_BORDER 0
#define EPS 1E-9
#define CAMERA_SCALE_X 0
#define CAMERA_SCALE_Y 0
#define ALIVE 1
#define TIMER_TEXT_SIZE 50
#define AMMUNITION_TEXT_SIZE 30
#define HERO_START_AMUNITION 30
#define TMP_PLATFORM_LIFE_TIME 1000
#define MAP_SIZE 5
#define MAP_TYPE_COUNT 5
#define POISON_CD 1500
#define POISON_DMG 1

#pragma region HERO_STATS

#define HERO_HP 100
#define HERO_DAMAGE 40
#define HERO_SHOOT_DAMAGE 50
#define HERO_DASH_CD 700
#define HERO_SPEED 7
#define HERO_BULLET_SPEED 21
#define HERO_ATACK_CD 300
#define HERO_SHOOT_CD 300 
#define HERO_BULLET_WIDTH 18 
#define HERO_BULLET_HIGHT 4 
#define HERO_AFTER_ATACK_PROTECTION 500 
#define DMG_BUFF_DUARATION 5000
#define DMG_BUFF_PERCENT 1.30
#define ITEM_COUNT 10
#define START_MONEY 100

#define DMG_POTION_DROP_CHANCE 90
#define RUBBER_REBOUND_COUNT 3;
#pragma endregion

#pragma region BARREL_STATS

#define BARREL_HP 50
#define BARREL_DMG 10
#define BARREL_SPEED 0
#define BARREL_ATACK_CD 0
#define BARREL_TRIGGERED_DISTANCE 50
#define BARREL_REAWRD 50

#pragma endregion

#pragma region BEAVER_STATS

#define BEAVER_HP 100
#define BEAVER_DMG 10
#define BEAVER_SPEED 8
#define BEAVER_ATACK_CD 1500
#define BEAVER_TRIGGERED_DISTANCE 100
#define BEAVER_REAWRD 200

#pragma endregion

#pragma region TOWER_STATS

#define TOWER_HP 200
#define TOWER_DMG 10
#define TOWER_SPEED 8
#define TOWER_ATACK_CD 3000
#define TOWER_TRIGGERED_DISTANCE 200
#define TOWER_REAWRD 300

#pragma endregion

#pragma region KRAB_STATS

#define KRAB_HP 200
#define KRAB_DMG 5
#define KRAB_SPEED 3
#define KRAB_ATACK_CD 1000
#define KRAB_TRIGGERED_DISTANCE 100
#define KRAB_REAWRD 200

#pragma endregion

#pragma region DIGIT_STATS

#define DIGIT_HP 50
#define DIGIT_DMG 50
#define DIGIT_SPEED 0
#define DIGIT_ATACK_CD 1000
#define DIGIT_TRIGGERED_DISTANCE 50
#define DIGIT_REAWRD 100

#pragma endregion

#pragma region TRAPS_STATS

#define TRAP_DART_WIDTH 40
#define TRAP_DART_HIGHT 18 
#define TRAP_DART_DMG 45 
#define TRAP_SPIKES_DMG 5 
#define TRAPS_BULLET_SPEED 20
#define TRAPS_DART_CD 5000


#pragma endregion

#pragma region TEXTURE_FLAGS

#define INIT 1
#define DONT_TOUCH 2
#define DELETE 3

#pragma endregion

struct mainWindow
{
	int w, h;
	float scaleX, scaleY;
};

typedef enum GameState {
	IN_GAME,
	SETTINGS,
	CREDITS,
	QUIT,
	MAIN_MENU,
	PAUSE_MENU
};

struct Settings
{
	int volume;
	int skin;
};

struct twoParam
{
	int i, j;
};

struct mainStatistic
{
	int startHP;
	int DMG;
	int HP;
	bool alive;
	int ammunition;
	int Shoot_DMG;
	int money;
	int souls;
};

struct mainPhysics
{
	float xMoveL, xMoveR, yMove;
	int gazeDirection;
	float speed;
	float gravity;
	float accelerationY;
	float accelerationX;
	float impulse;
	bool onBorder;
};

struct statusEffect
{
	int dashCD, timeDashCD;
	int atackCD, timeAtackCD;
	int shootCD, timeShootCD;
	SDL_Point camersScale;
	bool underAtack;
	int afterAtackResist;
	int lastDamage;
	bool poisoned;
	int poisonLastDamage;
};

struct mainRenderer
{
	SDL_Texture* texture;
	SDL_Rect textureSize;
	SDL_Rect frame;
	int frameTime;
	int frameCount;
	int angel;
};

struct heroAnimation
{
	mainRenderer com;
	mainRenderer run;
	mainRenderer punch;
	mainRenderer shoot;
	mainRenderer bullet;
	mainRenderer rubber_bullet;
	mainRenderer ball;
	mainRenderer DMG_Buff;
	mainRenderer dark;
};

struct enemysAnimation
{
	mainRenderer run;
	mainRenderer atack;
	mainRenderer preAtack;
	mainRenderer acid_effect;
	mainRenderer bullet;
};

struct mainShoot
{
	bool alive;
	int bulletSpeed;
	SDL_Point shootAtackCentere;
	int rebound_count;
	int lastShoot;
};

struct mainBattle
{
	bool commonAtack;
	SDL_Point commonAtackCentere;
	bool shootAtack;
	mainShoot shoot[10];
};

struct mainBuffs
{
	int buffDuaration;

	bool buffDMGactive;
	int buffDMGstart;
	double buffDMGpercent;

	bool buffLuckyActive;
	int buffLuckyStart;
	double buffLuckyPercent;

	bool buffSpeedActive;
	int buffSpeedStart;
	double buffSpeedPercent;

	bool itemRubberBulletActive;

	bool itemBallActive;

	bool itemAcid;
};

struct mainKeys
{
	bool pressed_S;
	bool pressed_Space;
	bool pressed_E;
	int time_pressed_S;
};


struct mainHero
{
	SDL_Point position;
	SDL_Rect hitbox;
	SDL_Rect atack_hitbox;
	SDL_Rect texture_rect;
	mainPhysics physic;
	statusEffect effect;
	heroAnimation animation;
	mainBattle battle;
	mainStatistic status;
	int animationType;
	mainBuffs buffs;
	mainKeys keys;
	twoParam curRoom;
	int money;
};
/*
0 - стойка
1 - бег
2 - атака мечом
3 - атака пистолетом
4 - дэш
5 - нахождение в воздухе
6 - преатака
*/


struct mainBorders
{
	int type;
	SDL_Rect bordersHitbox;
	int timer;
	int alive;
	bool active;
};

struct mainEnemys
{
	int type;
	int distanceToHero;
	SDL_Point position;
	SDL_Rect hitbox;
	mainPhysics physic;
	statusEffect effect;
	mainStatistic status;
	enemysAnimation animation;
	bool triggered;
	int triggeredDistance;
	int animation_type;
	mainShoot shoot;
	int reward;
};

struct mainItems
{
	int type;
	int dop_type;
	bool alive;
	int cost;
	SDL_Point position;
	SDL_Rect hitbox;
	mainRenderer render;
	SDL_Rect grab_zone;
};

struct mainTraps
{
	int type;
	SDL_Rect hitbox;
	mainShoot shoot;
	bool gazeDirection;
	mainRenderer render;
	bool triggered;
	int DMG;
	int lastShoot;
	int cooldown;
};

struct mainRoom
{
	int type;
	bool top, down, left, right;
};

struct mainTextureSkill
{
	mainRenderer figure;
	mainRenderer iconHealth;
	mainRenderer iconStrength;
	mainRenderer iconLucky;
	mainRenderer iconAmmo;
	mainRenderer iconWeapon;
	mainRenderer iconPotion;
};

int inline GetNumInRange(int leftBord, int rightBord)
{
	return rand() % (rightBord - leftBord + 1) + leftBord;
}