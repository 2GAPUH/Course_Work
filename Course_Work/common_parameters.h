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
#define SPEED 7
#define GRAVITY 10
#define ACCELERATION_Y 1
#define ACCELERATION_X 1
#define IMPULSE 0
#define ON_BORDER 0


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

struct mainHero
{
	SDL_Point position;
	SDL_Rect hitbox;
	mainPhysics physic;
};
