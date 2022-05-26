#include "Weapon.h"

//コンストラクタの定義
Weapon::Weapon(int weaponType) {
	this->weaponType = weaponType;
	tipTransform.x = 0;
	tipTransform.y = 0;
	tipTransform.r = 16;

	angle = 0;

	rootTransform = {0 , 0 , 0};
	rootScreen = {0 , 0};
	tipScreen = {0 , 0};
	speed = {5 , 0 , 0 , 0 , 0};
	isShot = false;
}

//デストラクタの定義
Weapon::~Weapon() {

}

//メンバ関数
//更新関数
void Weapon::Update(double x , double y , double angle , Scroll scroll,int map[MAP_HEIGHT][MAP_WIDTH]) {

	if (weaponType == SROWD) {
		this->angle = angle;

		rootTransform.x = x + cos(angle) * tipTransform.r;
		rootTransform.y = y + sin(angle) * tipTransform.r;

		tipTransform.x = x + cos(angle) * tipTransform.r * 3;
		tipTransform.y = y + sin(angle) * tipTransform.r * 3;

	}

	if (weaponType == ARROW) {

		if (isShot == false) {
			this->angle = angle;

			rootTransform.x = x + cos(angle) * tipTransform.r;
			rootTransform.y = y + sin(angle) * tipTransform.r;

			tipTransform.x = x + cos(angle) * tipTransform.r * 3;
			tipTransform.y = y + sin(angle) * tipTransform.r * 3;

		}

		else if (isShot == true) {
			speed.x = cos(angle) * speed.fixedValue;
			speed.y = sin(angle) * speed.fixedValue;

			tipTransform.x += speed.x;
			tipTransform.y += speed.y;

			rootTransform.x += speed.x;
			rootTransform.y += speed.y;

			if (map[(int)tipTransform.y / MAP_SIZE][(int)tipTransform.x / MAP_SIZE] == 1) {
				isShot = false;
			}
		}

	}
	tipScreen.x = tipTransform.x - scroll.x;
	tipScreen.y = tipTransform.y - scroll.y;

	rootScreen.x = rootTransform.x - scroll.x;
	rootScreen.y = rootTransform.y - scroll.y;

}

//セッター
void Weapon::SetIsShot(int isShot) {
	this->isShot = isShot;
}

//描画関数
void Weapon::Draw() {
	if (weaponType == SROWD) {
		DrawTriangle(tipScreen.x , tipScreen.y ,
					 rootScreen.x + cos(angle + PI / 2) * 7 , rootScreen.y + sin(angle + PI / 2) * 7 ,
					 rootScreen.x - cos(angle + PI / 2) * 7 , rootScreen.y - sin(angle + PI / 2) * 7 ,
					 GetColor(200 , 50 , 50) ,
					 true);
	}

	else if (weaponType == ARCHER) {
		if (isShot == true) {
			DrawTriangle(tipScreen.x , tipScreen.y ,
						 rootScreen.x + cos(angle + PI / 2) * 4 , rootScreen.y + sin(angle + PI / 2) * 4 ,
						 rootScreen.x - cos(angle + PI / 2) * 4 , rootScreen.y - sin(angle + PI / 2) * 4 ,
						 GetColor(200 , 50 , 50) ,
						 true);
		}
	}
}