#include "Weapon.h"

//コンストラクタの定義
Weapon::Weapon(int weaponType) {
	this->weaponType = weaponType;
	tipTransform.pos.x = 0;
	tipTransform.pos.y = 0;
	tipTransform.r = 0;

	angle = 0;

	rootTransform = {{0 , 0} , 0};
	rootScreen = {0 , 0};
	tipScreen = {0 , 0};
	speed = {0 , 0 , 0 , 0 , 0 , 0};
	isShot = false;
}

//デストラクタの定義
Weapon::~Weapon() {

}

//メンバ関数
//更新関数
void Weapon::Update(Transform transform , Scroll scroll , int map[MAP_HEIGHT][MAP_WIDTH]) {

	if (weaponType == SROWD) {
		this->angle = angle;

		rootTransform.pos.x = transform.pos.x + (float)cos(angle) * transform.r;
		rootTransform.pos.y = transform.pos.y + (float)sin(angle) * transform.r;

		tipTransform.pos.x = transform.pos.x + (float)cos(angle) * (tipTransform.r + transform.r);
		tipTransform.pos.y = transform.pos.y + (float)sin(angle) * (tipTransform.r + transform.r);

	}

	if (weaponType == BOW) {

		if (isShot == false) {
			this->angle = angle;

			rootTransform.pos.x = transform.pos.x + (float)cos(angle) * transform.r;
			rootTransform.pos.y = transform.pos.y + (float)sin(angle) * transform.r;

			tipTransform.pos.x = transform.pos.x + (float)cos(angle) * (tipTransform.r + transform.r);
			tipTransform.pos.y = transform.pos.y + (float)sin(angle) * (tipTransform.r + transform.r);

		}

		else if (isShot == true) {
			speed.x = (float)cos(angle) * speed.fixedValue;
			speed.y = (float)sin(angle) * speed.fixedValue;

			tipTransform.pos.x += speed.x;
			tipTransform.pos.y += speed.y;

			rootTransform.pos.x += speed.x;
			rootTransform.pos.y += speed.y;

			if (map[(int)tipTransform.pos.y / MAP_SIZE][(int)tipTransform.pos.x / MAP_SIZE] == 1) {
				isShot = false;
			}
		}

	}
	tipScreen.x = tipTransform.pos.x - scroll.x;
	tipScreen.y = tipTransform.pos.y - scroll.y;

	rootScreen.x = rootTransform.pos.x - scroll.x;
	rootScreen.y = rootTransform.pos.y - scroll.y;

}

//アクセッサ
void Weapon::SetIsShot(int isShot) {
	this->isShot = isShot;
}

Speed  Weapon::GetSpeed() {
	return speed;
}

void  Weapon::SetSpeedFixedValue(float fixedValue) {
	speed.fixedValue = fixedValue;
}

void Weapon::SetAngle(float angle) {
	this->angle = angle;
}

float Weapon::GetLength() {
	return tipTransform.r;
}

void Weapon::SetLength(float length) {
	tipTransform.r = length;
}

//描画関数
void Weapon::Draw() {
	if (weaponType == SROWD) {
		DrawTriangle(tipScreen.x , tipScreen.y ,
					 rootScreen.x + (float)cos(angle + PI / 2) * 7 , rootScreen.y + (float)sin(angle + PI / 2) * 7 ,
					 rootScreen.x - (float)cos(angle + PI / 2) * 7 , rootScreen.y - (float)sin(angle + PI / 2) * 7 ,
					 GetColor(200 , 50 , 50) ,
					 true);
	}

	else if (weaponType == ARCHER) {
		if (isShot == true) {
			DrawTriangle(tipScreen.x , tipScreen.y ,
						 rootScreen.x + (float)cos(angle + PI / 2) * 4 , rootScreen.y + (float)sin(angle + PI / 2) * 4 ,
						 rootScreen.x - (float)cos(angle + PI / 2) * 4 , rootScreen.y - (float)sin(angle + PI / 2) * 4 ,
						 GetColor(200 , 50 , 50) ,
						 true);
		}
	}
}