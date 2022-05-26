#pragma once

typedef struct Transform {

	double x;
	double y;
	double r;

}Transform;

typedef struct Screen {

	double x;
	double y;

}Screen;

typedef struct Speed {

	double fixedValue;
	double x;
	double y;
	double tmpX;
	double tmpY;
	double initialspeedY;

}Speed;

typedef struct MapColider {

	int rightTopX;
	int rightTopY;
	int rightBottomX;
	int rightBottomY;
	int leftTopX;
	int leftTopY;
	int leftBottomX;
	int leftBottomY;

}MapColider;

typedef struct Scroll {

	double x;
	double y;
	double MAX_X;
	double MAX_Y;

}Scrool;

typedef struct PlayerStruct {

	Transform transform;
	Screen screen;
	Speed speed;
	MapColider mapColider;
	int isJump;
	int isDoubleJump;
	int isInTheAir;
	int AirTimer;

}PlayerStruct;

typedef struct EnemyStruct {

	Transform transform;
	Screen screen;
	Speed speed;
	MapColider mapColider;
	double angle;
	double distanceToPlayer;
	double sensingRenge;
	double attackRenge;
	int type;
	int isFacingRight;
	int isInTheAir;
	int isShotArrow;
	int AirTimer;
	int motionTimer;
	int motionPhase;
	int isAttack;
	int attackCT;
	int weaponType;

}EnemyStruct;