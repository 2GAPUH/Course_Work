#pragma once

#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define FPS 60
#define HERO_WIDHT 40
#define HERO_HEIGHT 40
#define X_MOVE_L 0
#define X_MOVE_R 0
#define Y_MOVE 0
#define GAZE_DIRECTION 0
#define SPEED 9
#define GRAVITY 15
#define ACCELERATION_Y 1
#define ACCELERATION_X 1
#define IMPULSE 0
#define ON_BORDER 0
#define EPS 1E-9
#define DASH_CD 700
#define CAMERA_SCALE_X 0
#define CAMERA_SCALE_Y 0


struct windowSize
{
	int w, h;
} window;

struct mainPhysics
{
	float xMoveL, xMoveR, yMove;
	bool gazeDirection;
	float speed;
	float gravity;
	float accelerationY;
	float accelerationX;
	float impulse;
	bool onBorder;
};

struct statusEffect
{
	int dashCD, timeCD;
	SDL_Point camersScale;
};

struct mainHero
{
	SDL_Point position;
	SDL_Rect hitbox;
	mainPhysics physic;
	statusEffect effect;
};
