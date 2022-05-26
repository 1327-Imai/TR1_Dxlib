#include "Enemy.h"

//コンストラクタの定義
Enemy::Enemy(int enemyType) {

	enemy.type = enemyType;

	if (enemy.type == SLIME) {
		//transform
		enemy.transform.x = MAP_SIZE * 10;
		enemy.transform.y = MAP_SIZE * ((double)MAP_HEIGHT - 3.5);
		enemy.transform.r = 15;

		//speed
		enemy.speed.fixedValue = 1;

		//range
		enemy.sensingRenge = 10 * MAP_SIZE;
		enemy.attackRenge = 6 * MAP_SIZE;

		//weapon
		enemy.weaponType = NOWEAPON;

	}

	else if (enemy.type == FENCER) {
		//transform
		enemy.transform.x = MAP_SIZE * 20;
		enemy.transform.y = MAP_SIZE * ((double)MAP_HEIGHT - 3.5);
		enemy.transform.r = 15;

		//speed
		enemy.speed.fixedValue = 2;

		//range
		enemy.sensingRenge = 10 * MAP_SIZE;
		enemy.attackRenge = 5 * MAP_SIZE;

		//angle
		enemy.angle = PI * 5 / 6;

		//weapon
		enemy.weaponType = SROWD;
	}

	else if (enemy.type == ARCHER) {
		//transform
		enemy.transform.x = MAP_SIZE * 30;
		enemy.transform.y = MAP_SIZE * ((double)MAP_HEIGHT - 3.5);
		enemy.transform.r = 15;

		//speed
		enemy.speed.fixedValue = 2;

		//range
		enemy.sensingRenge = 15 * MAP_SIZE;
		enemy.attackRenge = 9 * MAP_SIZE;

		//angle
		enemy.angle = PI;

		//weapon
		enemy.weaponType = ARROW;
		enemy.isShotArrow = false;
	}

	//weapon
	weapon = new Weapon(enemy.weaponType);

	//others
	enemy.AirTimer = 0;
	enemy.isInTheAir = 0;
	enemy.isFacingRight = false;
	enemy.attackCT = 0;
	enemy.motionTimer = 0;
	enemy.motionPhase = 0;
}

Enemy::Enemy(EnemyStruct enemy) {
	this->enemy = enemy;

	weapon = new Weapon(enemy.weaponType);
}

//デストラクタの定義
Enemy::~Enemy() {
	delete weapon;
}

//メンバ関数
void Enemy::Update(Player player , int  map[MAP_HEIGHT][MAP_WIDTH] , Scroll& scroll) {

	enemy.speed.x = 0;
	enemy.speed.y = 0;
	enemy.speed.tmpX = enemy.speed.fixedValue;
	enemy.speed.tmpY = 0;

	//敵の本来の四隅の縦方向の座標を取得する
	enemy.mapColider.leftTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
	enemy.mapColider.leftBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;
	enemy.mapColider.rightTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
	enemy.mapColider.rightBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;

	//敵の本来の四隅の横方向の座標を取得する
	enemy.mapColider.rightTopX = ((int)enemy.transform.x + (int)enemy.transform.r - 1) / MAP_SIZE;
	enemy.mapColider.rightBottomX = ((int)enemy.transform.x + (int)enemy.transform.r - 1) / MAP_SIZE;
	enemy.mapColider.leftTopX = ((int)enemy.transform.x - (int)enemy.transform.r) / MAP_SIZE;
	enemy.mapColider.leftBottomX = ((int)enemy.transform.x - (int)enemy.transform.r) / MAP_SIZE;

	if (enemy.isInTheAir == false) {

		if (map[enemy.mapColider.rightBottomY + 1][enemy.mapColider.rightBottomX] == 0 &&
			map[enemy.mapColider.leftBottomY + 1][enemy.mapColider.leftBottomX] == 0) {
			enemy.isInTheAir = true;
			enemy.speed.initialspeedY = 0;
		}

	}

	enemy.distanceToPlayer = sqrt(
		pow(player.GetPlyaerTransformX() - enemy.transform.x , 2) +
		pow(player.GetPlyaerTransformY() - enemy.transform.y , 2)
	);

	if (enemy.distanceToPlayer <= enemy.attackRenge) {
		if (enemy.attackCT <= 0) {
			enemy.isAttack = true;
		}
	}

	if (enemy.isAttack == true) {
		if (enemy.type == SLIME) {
			AttackOfSlime(map);
		}

		else if (enemy.type == FENCER) {
			AttackOfFencer(map);
		}

		else if (enemy.type == ARCHER) {
			AttackOfArcher(map , player);
		}
	}

	else {
		if (enemy.distanceToPlayer <= enemy.sensingRenge) {

			if (enemy.transform.x < player.GetPlyaerTransformX()) {
				enemy.isFacingRight = true;
			}
			else {
				enemy.isFacingRight = false;
			}

			if (enemy.type == SLIME) {
				MoveOfSlime(map);
			}

			else if (enemy.type == FENCER) {
				MoveOfFencer(map);
			}

			else if (enemy.type == ARCHER) {
				MoveOfArcher(map , player);
			}
		}
	}

	if (enemy.isInTheAir == true) {
		if (enemy.isAttack == true) {
			if (enemy.type != SLIME) {
				enemy.speed.tmpY = enemy.speed.initialspeedY + (G / 50) * enemy.AirTimer;

				//現在のプレイヤーの四隅の縦方向の座標に一回分進んだ場合の座標を取得
				enemy.mapColider.leftTopY = ((int)enemy.transform.y + enemy.speed.tmpY - (int)enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.rightTopY = ((int)enemy.transform.y + enemy.speed.tmpY - (int)enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomY = ((int)enemy.transform.y + enemy.speed.tmpY + (int)enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightBottomY = ((int)enemy.transform.y + enemy.speed.tmpY + (int)enemy.transform.r - 1) / MAP_SIZE;

				if (enemy.speed.tmpY < 0) {
					if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == 0 &&
						map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == 0) {
						enemy.speed.y = enemy.speed.tmpY;
					}
					else {//天井にあたったら
						enemy.AirTimer = 0;
						enemy.speed.initialspeedY = 0;
					}
				}

				else if (0 <= enemy.speed.tmpY) {
					if (map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == 0 &&
						map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == 0) {
						enemy.speed.y = enemy.speed.tmpY;
					}
					else {
						//地面についたら
						enemy.isInTheAir = false;
						enemy.AirTimer = 0;
						enemy.transform.y = enemy.mapColider.rightBottomY * MAP_SIZE - 16;
					}
				}

				//実際に敵を進める
				enemy.transform.y += enemy.speed.y;

				//敵の本来の四隅の縦方向の座標を取得する
				enemy.mapColider.leftTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.rightBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;

				enemy.AirTimer++;
			}
		}
	}

	if (0 <= enemy.attackCT) {
		enemy.attackCT--;
	}
	enemy.screen.x = enemy.transform.x - scroll.x;
	enemy.screen.y = enemy.transform.y - scroll.y;

	weapon->Update(enemy.transform.x , enemy.transform.y , enemy.angle , scroll , map);

}

void Enemy::Draw() {

#pragma region//デバッグ用:攻撃範囲の表示
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA , 30);
	//DrawCircle(enemy.screen.x ,
	//		   enemy.screen.y ,
	//		   enemy.sensingRenge ,
	//		   GetColor(200 , 0 , 0) ,
	//		   true);

	//DrawCircle(enemy.screen.x ,
	//		   enemy.screen.y ,
	//		   enemy.attackRenge ,
	//		   GetColor(0 , 200 , 0) ,
	//		   true);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND , 30);
#pragma endregion////デバッグ用:攻撃範囲の表示

	if (enemy.type == SLIME) {
		DrawOfSlime();
	}
	else if (enemy.type == FENCER) {
		DrawOfFencer();
	}
	else if (enemy.type == ARCHER) {
		DrawOfArcher();
	}

	weapon->Draw();

}


#pragma region//スライム

void Enemy::MoveOfSlime(int  map[MAP_HEIGHT][MAP_WIDTH]) {

	if (enemy.isInTheAir == false) {

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
			if (enemy.isFacingRight == true) {//右を向いていなかったら
				//現在のプレイヤーの右上、右下の頂点に一回分右方向に進んだ場合の座標を取得
				enemy.mapColider.rightTopX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが右に行くように値を入れる
				if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == NONE && map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == NONE) {
					enemy.speed.x += enemy.speed.fixedValue;
				}
			}
		}

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
			if (enemy.isFacingRight == false) {//右を向いなかったら
				//現在のプレイヤーの左上、左下の頂点に一回分左方向に進んだ場合の座標を取得
				enemy.mapColider.leftTopX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが左に行くように値を入れる
				if (map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == NONE && map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == NONE) {
					enemy.speed.x -= enemy.speed.fixedValue;
				}
			}
		}

		//実際にプレイヤーを進める
		enemy.transform.x += enemy.speed.x;

		//プレイヤーの本来の四隅の横方向の座標を取得する
		enemy.mapColider.rightTopX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.leftTopX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
	}
}

void Enemy::AttackOfSlime(int  map[MAP_HEIGHT][MAP_WIDTH]) {

	if (enemy.motionPhase == 0) {
		enemy.speed.initialspeedY = -3;
		enemy.speed.fixedValue = 5;
		enemy.isInTheAir = true;
		enemy.AirTimer = 0;
		enemy.motionTimer = 25;
		enemy.motionPhase++;
	}

	else if (enemy.motionPhase == 1) {
		enemy.motionTimer--;
		if (enemy.motionTimer <= 0) {
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 2) {

		enemy.speed.tmpY = enemy.speed.initialspeedY + (G / 50) * enemy.AirTimer;

		//現在のスライムの四隅の縦方向の座標に一回分進んだ場合の座標を取得
		enemy.mapColider.leftTopY = ((int)enemy.transform.y + enemy.speed.tmpY - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.rightTopY = ((int)enemy.transform.y + enemy.speed.tmpY - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomY = ((int)enemy.transform.y + enemy.speed.tmpY + (int)enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightBottomY = ((int)enemy.transform.y + enemy.speed.tmpY + (int)enemy.transform.r - 1) / MAP_SIZE;

		if (enemy.speed.tmpY < 0) {
			if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == 0 &&
				map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == 0) {
				enemy.speed.y = enemy.speed.tmpY;
			}
			//天井に当たったとき
			else {
				enemy.AirTimer = 0;
				enemy.speed.initialspeedY = 0;
			}
		}

		else if (0 <= enemy.speed.tmpY) {
			if (map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == 0 &&
				map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == 0) {
				enemy.speed.y = enemy.speed.tmpY;
			}
			//地面に付いたら
			else {
				enemy.isInTheAir = false;
				enemy.AirTimer = 0;
				enemy.transform.y = enemy.mapColider.rightBottomY * MAP_SIZE - 16;
				enemy.motionPhase++;
				enemy.motionTimer = 30;
			}
		}

		//実際にスライムを進める
		enemy.transform.y += enemy.speed.y;

		//スライムの本来の四隅の縦方向の座標を取得する
		enemy.mapColider.leftTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightTopY = ((int)enemy.transform.y - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.rightBottomY = ((int)enemy.transform.y + (int)enemy.transform.r - 1) / MAP_SIZE;

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//スライムが右端より左にいるとき
			if (enemy.isFacingRight == true) {//右を向いていたら
				//現在のスライムの右上、右下の頂点に一回分右方向に進んだ場合の座標を取得
				enemy.mapColider.rightTopX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればスライムの横方向のスピードが右に行くように値を入れる
				if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == NONE && map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == NONE) {
					enemy.speed.x += enemy.speed.fixedValue;
				}
				else {//壁に当たったら
					enemy.transform.x = enemy.mapColider.rightBottomX * MAP_SIZE - 16;
					enemy.speed.fixedValue = 0;
				}
			}
		}

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//スライムが右端より左にいるとき
			if (enemy.isFacingRight == false) {//右を向いなかったら
				//現在のスライムの左上、左下の頂点に一回分左方向に進んだ場合の座標を取得
				enemy.mapColider.leftTopX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればスライムの横方向のスピードが左に行くように値を入れる
				if (map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == NONE && map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == NONE) {
					enemy.speed.x -= enemy.speed.fixedValue;
				}
				else {//壁に当たったら
					enemy.transform.x = enemy.mapColider.rightBottomX * MAP_SIZE + 15;
					enemy.speed.fixedValue = 0;
				}
			}
		}

		//実際にスライムを進める
		enemy.transform.x += enemy.speed.x;

		//スライムの本来の四隅の横方向の座標を取得する
		enemy.mapColider.rightTopX = ((int)enemy.transform.x + (int)enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightBottomX = ((int)enemy.transform.x + (int)enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.leftTopX = ((int)enemy.transform.x - (int)enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomX = ((int)enemy.transform.x - (int)enemy.transform.r) / MAP_SIZE;

		enemy.AirTimer++;
	}
	else if (enemy.motionPhase == 3) {
		enemy.motionTimer--;
		if (enemy.motionTimer <= 0) {
			enemy.isAttack = false;
			enemy.motionPhase = 0;
			enemy.attackCT = 5;
			enemy.speed.fixedValue = 1;
		}
	}
}

void Enemy::DrawOfSlime() {
	DrawCircle(enemy.screen.x ,
			   enemy.screen.y ,
			   enemy.transform.r ,
			   GetColor(200 , 50 , 50) ,
			   true);

	if (enemy.isFacingRight == true) {

		DrawBox(enemy.screen.x - enemy.transform.r / 2 , enemy.screen.y + enemy.transform.r ,
				enemy.screen.x , enemy.screen.y - enemy.transform.r ,
				GetColor(200 , 50 , 50) ,
				true);

		DrawTriangle(enemy.screen.x - enemy.transform.r / 2 , enemy.screen.y + enemy.transform.r ,
					 enemy.screen.x - enemy.transform.r / 2 , enemy.screen.y - enemy.transform.r ,
					 enemy.screen.x - enemy.transform.r * 2 , enemy.screen.y + enemy.transform.r ,
					 GetColor(200 , 50 , 50) ,
					 true);
	}

	else {

		DrawBox(enemy.screen.x + enemy.transform.r / 2 , enemy.screen.y + enemy.transform.r ,
				enemy.screen.x , enemy.screen.y - enemy.transform.r ,
				GetColor(200 , 50 , 50) ,
				true);

		DrawTriangle(enemy.screen.x + enemy.transform.r / 2 , enemy.screen.y + enemy.transform.r ,
					 enemy.screen.x + enemy.transform.r / 2 , enemy.screen.y - enemy.transform.r ,
					 enemy.screen.x + enemy.transform.r * 2 , enemy.screen.y + enemy.transform.r ,
					 GetColor(200 , 50 , 50) ,
					 true);
	}
}

#pragma endregion//スライム

#pragma region//フェンサー

void Enemy::MoveOfFencer(int  map[MAP_HEIGHT][MAP_WIDTH]) {

	if (enemy.isInTheAir == false) {

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
			if (enemy.isFacingRight == true) {//右を向いていなかったら
				//現在のプレイヤーの右上、右下の頂点に一回分右方向に進んだ場合の座標を取得
				enemy.mapColider.rightTopX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが右に行くように値を入れる
				if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == NONE && map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == NONE) {
					enemy.speed.x += enemy.speed.fixedValue;
				}
			}
		}

		if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
			if (enemy.isFacingRight == false) {//右を向いなかったら
				//現在のプレイヤーの左上、左下の頂点に一回分左方向に進んだ場合の座標を取得
				enemy.mapColider.leftTopX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
				//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが左に行くように値を入れる
				if (map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == NONE && map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == NONE) {
					enemy.speed.x -= enemy.speed.fixedValue;
				}
			}
		}

		//実際にプレイヤーを進める
		enemy.transform.x += enemy.speed.x;

		//プレイヤーの本来の四隅の横方向の座標を取得する
		enemy.mapColider.rightTopX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
		enemy.mapColider.leftTopX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
		enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
	}

	if (enemy.isFacingRight == true) {
		enemy.angle = PI / 6;
	}
	else {
		enemy.angle = PI * 5 / 6;
	}

}

void Enemy::AttackOfFencer(int  map[MAP_HEIGHT][MAP_WIDTH]) {
	if (enemy.motionPhase == 0) {
		if (enemy.distanceToPlayer < enemy.attackRenge / 2) {
			enemy.motionPhase = 4;
		}
		else {
			enemy.motionPhase = 1;
		}
	}
#pragma region//攻撃パターン1
	else if (enemy.motionPhase == 1) {
		enemy.speed.tmpX = (enemy.speed.fixedValue + 1) - (0.04 * (double)enemy.motionTimer);

		if (0 < enemy.speed.tmpX) {
			if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
				if (enemy.isFacingRight == true) {//右を向いていたら
					//現在のプレイヤーの右上、右下の頂点に一回分右方向に進んだ場合の座標を取得
					enemy.mapColider.rightTopX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
					enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.speed.tmpX + enemy.transform.r - 1) / MAP_SIZE;
					//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが右に行くように値を入れる
					if (map[enemy.mapColider.rightTopY][enemy.mapColider.rightTopX] == NONE && map[enemy.mapColider.rightBottomY][enemy.mapColider.rightBottomX] == NONE) {
						enemy.speed.x += enemy.speed.tmpX;
					}
					enemy.angle -= (PI * 4 / 6) / 75;
				}
			}

			if (enemy.transform.x + enemy.transform.r < MAP_WIDTH * MAP_SIZE) {//プレイヤーが右端より左にいるとき
				if (enemy.isFacingRight == false) {//右を向いなかったら
					//現在のプレイヤーの左上、左下の頂点に一回分左方向に進んだ場合の座標を取得
					enemy.mapColider.leftTopX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
					enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.speed.tmpX - enemy.transform.r) / MAP_SIZE;
					//取得した座標の位置のマップチップがNONEであればプレイヤーの横方向のスピードが左に行くように値を入れる
					if (map[enemy.mapColider.leftTopY][enemy.mapColider.leftTopX] == NONE && map[enemy.mapColider.leftBottomY][enemy.mapColider.leftBottomX] == NONE) {
						enemy.speed.x -= enemy.speed.tmpX;
					}
					enemy.angle += (PI * 4 / 6) / 75;
				}
			}

			//実際にプレイヤーを進める
			enemy.transform.x += enemy.speed.x;

			//プレイヤーの本来の四隅の横方向の座標を取得する
			enemy.mapColider.rightTopX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
			enemy.mapColider.rightBottomX = (enemy.transform.x + enemy.transform.r - 1) / MAP_SIZE;
			enemy.mapColider.leftTopX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
			enemy.mapColider.leftBottomX = (enemy.transform.x - enemy.transform.r) / MAP_SIZE;
		}

		else {
			enemy.motionPhase++;
			enemy.motionTimer = 0;
		}

		enemy.motionTimer++;
	}

	else if (enemy.motionPhase == 2) {
		if (enemy.motionTimer <= 4) {
			if (enemy.isFacingRight == true) {
				enemy.angle += (PI / 6);
			}
			else {
				enemy.angle -= (PI / 6);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 40;
			enemy.motionPhase++;
		}
	}
	else if (enemy.motionPhase == 3) {
		if (0 <= enemy.motionTimer) {
			enemy.motionTimer--;
		}
		else {
			enemy.motionPhase = 0;
			enemy.motionTimer = 0;
			enemy.isAttack = false;
			enemy.attackCT = 5;
		}
	}
#pragma endregion//攻撃パターン1

#pragma region//攻撃パターン2

	else if (enemy.motionPhase == 4) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle -= (PI / 12);
			}
			else {
				enemy.angle += (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 0;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 5) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle += (PI / 12);
			}
			else {
				enemy.angle -= (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 0;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 6) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle -= (PI / 12);
			}
			else {
				enemy.angle += (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 0;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 7) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle += (PI / 12);
			}
			else {
				enemy.angle -= (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 0;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 8) {
		if (enemy.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.angle -= (PI / 12);
			}
			else {
				enemy.angle += (PI / 12);
			}
			enemy.motionTimer++;
		}
		else {
			enemy.motionTimer = 50;
			enemy.motionPhase++;
		}
	}

	else if (enemy.motionPhase == 9) {
		if (0 <= enemy.motionTimer) {
			enemy.motionTimer--;
			if (enemy.motionTimer <= 30) {
				if (enemy.isFacingRight == true) {
					enemy.angle += (PI * 6 / 12) / 28;
				}
				else {
					enemy.angle -= (PI * 6 / 12) / 28;
				}
			}
		}
		else {
			enemy.motionPhase = 0;
			enemy.motionTimer = 0;
			enemy.isAttack = false;
			enemy.attackCT = 5;
		}
	}

#pragma endregion//攻撃パターン2

}

void Enemy::DrawOfFencer() {

	DrawCircle(enemy.screen.x ,
			   enemy.screen.y ,
			   enemy.transform.r ,
			   GetColor(200 , 50 , 50) ,
			   true);

}

#pragma endregion//フェンサー

#pragma region//アーチャー

void Enemy::MoveOfArcher(int  map[MAP_HEIGHT][MAP_WIDTH] , Player player) {

	if (enemy.isFacingRight == true) {
		enemy.angle = 0;
	}
	else {
		enemy.angle = PI;
	}

}

void Enemy::AttackOfArcher(int  map[MAP_HEIGHT][MAP_WIDTH] , Player player) {

	if (enemy.motionPhase == 0) {
		enemy.motionTimer = 50;
		enemy.motionPhase++;
	}

	else if (enemy.motionPhase == 1) {
		enemy.angle = atan2((double)player.GetPlyaerTransformY() - enemy.transform.y ,
							(double)player.GetPlyaerTransformX() - enemy.transform.x);

		if (0 < enemy.motionTimer) {
			enemy.motionTimer--;
		}
		else {
			enemy.motionPhase++;
			enemy.motionTimer = 200;
			enemy.isShotArrow = true;
			weapon->SetIsShot(enemy.isShotArrow);
		}
	}

	else if (enemy.motionPhase == 2) {

		if (enemy.isShotArrow == true) {
			if (0 < enemy.motionTimer) {
				enemy.motionTimer--;
			}
			else {
				enemy.isShotArrow = false;
				weapon->SetIsShot(enemy.isShotArrow);
			}
		}
		else if (enemy.isShotArrow == false) {
			enemy.motionPhase = 0;
			enemy.attackCT = 100;
			enemy.isAttack = false;
		}

	}

}

void Enemy::DrawOfArcher() {

	DrawCircle(enemy.screen.x ,
			   enemy.screen.y ,
			   enemy.transform.r ,
			   GetColor(200 , 50 , 50) ,
			   true);

	DrawTriangle(enemy.screen.x + cos(enemy.angle) * enemy.transform.r * 2 ,
				 enemy.screen.y + sin(enemy.angle) * enemy.transform.r * 2 ,
				 enemy.screen.x + cos(enemy.angle) * enemy.transform.r + cos(enemy.angle + PI / 2) * enemy.transform.r ,
				 enemy.screen.y + sin(enemy.angle) * enemy.transform.r + sin(enemy.angle + PI / 2) * enemy.transform.r ,
				 enemy.screen.x + cos(enemy.angle) * enemy.transform.r - cos(enemy.angle + PI / 2) * enemy.transform.r ,
				 enemy.screen.y + sin(enemy.angle) * enemy.transform.r - sin(enemy.angle + PI / 2) * enemy.transform.r ,
				 GetColor(200 , 50 , 50) ,
				 false);

}

#pragma endregion//アーチャー