#include "Enemy.h"
#include "Component.h"

//コンストラクタの定義
Enemy::Enemy(int enemyType) {

	enemy.type = enemyType;

	if (enemy.type == SLIME) {
		//transform
		enemy.transform.pos.x = MAP_SIZE * 10;
		enemy.transform.pos.y = MAP_SIZE * ((float)MAP_HEIGHT - 3.5);
		enemy.transform.r = 15;

		//speed
		enemy.speed.fixedValue = 1;

		//attack
		enemy.attack.range = 5 * MAP_SIZE;
		enemy.attack.amplitude = PI / 180 * 30;

		enemy.attack.weaponType = NOWEAPON;
		weapon = new Weapon(enemy.attack.weaponType);

		//eye
		enemy.eye.range = 7 * MAP_SIZE;
		enemy.eye.fov = PI / 180 * 110;
	}

	else if (enemy.type == FENCER) {
		//transform
		enemy.transform.pos.x = MAP_SIZE * 20;
		enemy.transform.pos.y = MAP_SIZE * ((float)MAP_HEIGHT - 3.5);
		enemy.transform.r = 15;

		//speed
		enemy.speed.fixedValue = 2;

		//attack
		enemy.attack.weaponType = SROWD;

		enemy.attack.weaponAngle = PI * 5 / 6;
		enemy.attack.range = 5 * MAP_SIZE;
		enemy.attack.amplitude = PI / 180 * 30;

		weapon = new Weapon(enemy.attack.weaponType);
		weapon->SetLength(32.0f);

		//eye
		enemy.eye.range = 10 * MAP_SIZE;
		enemy.eye.fov = PI / 180 * 110;
	}

	else if (enemy.type == ARCHER) {
		//transform
		enemy.transform.pos.x = MAP_SIZE * 25.5;
		enemy.transform.pos.y = MAP_SIZE * ((float)MAP_HEIGHT - 5.5);
		enemy.transform.r = 15;

		//speed
		enemy.speed.fixedValue = 2;

		//attack
		enemy.attack.weaponType = BOW;
		enemy.attack.weaponAngle = PI;
		enemy.attack.isShot = false;

		enemy.attack.range = 10 * MAP_SIZE;
		enemy.attack.amplitude = PI / 180 * 110;

		weapon = new Weapon(enemy.attack.weaponType);
		weapon->SetSpeedFixedValue(10.0f);
		weapon->SetLength(32.0f);

		//eye
		enemy.eye.range = 15 * MAP_SIZE;
		enemy.eye.fov = PI / 180 * 110;

	}

	//others
	enemy.aerialController.timer = 0;
	enemy.aerialController.isInTheAir = 0;
	enemy.isFacingRight = false;
	enemy.attack.coolTime = 0;
	enemy.attack.motionTimer = 0;
	enemy.attack.motionPhase = 0;

	enemy.eye.x = enemy.transform.pos.x - 1;
	enemy.eye.y = enemy.transform.pos.y - 1;
}

Enemy::Enemy(EnemyStruct enemy) {
	this->enemy = enemy;

	weapon = new Weapon(enemy.attack.weaponType);
}

//デストラクタの定義
Enemy::~Enemy() {
	delete weapon;
}

//メンバ関数
void Enemy::Update(Player player , int  map[MAP_HEIGHT][MAP_WIDTH] , Scroll& scroll) {

	enemy.speed.x = 0;
	enemy.speed.y = 0;
	enemy.speed.tmpX = 0;
	enemy.speed.tmpY = 0;


	//プレイヤーとの距離を計算する
	enemy.distanceToPlayer = sqrt(
		pow(player.GetTransform().pos.x - enemy.transform.pos.x , 2) +
		pow(player.GetTransform().pos.y - enemy.transform.pos.y , 2)
	);

	//近く(マップチップ1マス居ない)にいれば
	if (enemy.distanceToPlayer < MAP_SIZE) {

		enemy.eye.x = player.GetTransform().pos.x;
		enemy.eye.y = player.GetTransform().pos.y;

	}

	LoadComponent(player , map);

	//プレイヤーとの距離が攻撃範囲より近ければ
	if (enemy.eye.isVisuable) {
		if (DetermineIsAttackHit()) {
			if (enemy.attack.coolTime <= 0) {
				//攻撃フラグをtrueにする
				enemy.attack.isAttack = true;
			}
		}
	}

	//攻撃フラグがtrueなら攻撃をする
	if (enemy.attack.isAttack == true) {
		Attack(player);
	}

	//そうでなければ
	else {
		NomalMove(player);
	}

	//重力の影響を受ける
	Component::AffectedByGravity(enemy.speed , enemy.aerialController);
	//マップチップとの当たり判定を行う
	Component::MapchipCollision(enemy.transform , enemy.speed , enemy.aerialController , map);

	enemy.transform.pos.x += enemy.speed.x;
	enemy.transform.pos.y += enemy.speed.y;

	//攻撃のクールタイムが0より大きいなら
	if (0 < enemy.attack.coolTime) {
		//クールタイムを減少
		enemy.attack.coolTime--;
	}

	//スクリーン座標を取得
	enemy.screen.x = enemy.transform.pos.x - scroll.x;
	enemy.screen.y = enemy.transform.pos.y - scroll.y;

	//武器の更新処理を行う
	weapon->Update(enemy.transform , scroll , map);


	DebugDraw(scroll);
}

int Enemy::DetermineIsAttackHit() {

	Vector2 targetPos = {enemy.target.x , enemy.target.y};

	Vector2 attackRangeVert1{
		enemy.transform.pos.x + (float)cos(enemy.attack.angle + enemy.attack.amplitude / 2) * enemy.attack.range ,
		enemy.transform.pos.y + (float)sin(enemy.attack.angle + enemy.attack.amplitude / 2) * enemy.attack.range
	};

	Vector2 attackRangeVert2{
		enemy.transform.pos.x + (float)cos(enemy.attack.angle) * enemy.attack.range ,
		enemy.transform.pos.y + (float)sin(enemy.attack.angle) * enemy.attack.range
	};

	Vector2 attackRangeVert3{
		enemy.transform.pos.x + (float)cos(enemy.attack.angle - enemy.attack.amplitude / 2) * enemy.attack.range ,
		enemy.transform.pos.y + (float)sin(enemy.attack.angle - enemy.attack.amplitude / 2) * enemy.attack.range
	};

	Vector2 crossA1 = enemy.transform.pos - attackRangeVert1;
	Vector2 crossA2 = attackRangeVert1 - attackRangeVert2;
	Vector2 crossA3 = attackRangeVert2 - attackRangeVert3;
	Vector2 crossA4 = attackRangeVert3 - enemy.transform.pos;

	Vector2 crossB1 = enemy.transform.pos - targetPos;
	Vector2 crossB2 = attackRangeVert1 - targetPos;
	Vector2 crossB3 = attackRangeVert2 - targetPos;
	Vector2 crossB4 = attackRangeVert3 - targetPos;

	if (crossA1.cross(crossB1) < 0 &&
		crossA2.cross(crossB2) < 0 &&
		crossA3.cross(crossB3) < 0 &&
		crossA4.cross(crossB4) < 0) {

		return 1;
	}
	else {
		return 0;
	}

}

void Enemy::NomalMove(Player player) {

	//プレイヤーが見えていれば
	if (enemy.eye.isVisuable) {

		//プレイヤーが右にいるなら右を向く
		if (enemy.transform.pos.x < player.GetTransform().pos.x) {
			enemy.isFacingRight = true;
		}
		//そうでないなら左を向く
		else {
			enemy.isFacingRight = false;
		}
		//それぞれ移動処理を行う
		if (enemy.type == SLIME) {
			NomalMoveOfSlime();
		}

		else if (enemy.type == FENCER) {
			NomalMoveOfFencer();
		}

		else if (enemy.type == ARCHER) {
			NomalMoveOfArcher(player);
		}
	}
}

void Enemy::Attack(Player player) {
	if (enemy.type == SLIME) {
		AttackOfSlime();
	}

	else if (enemy.type == FENCER) {
		AttackOfFencer(player);
	}

	else if (enemy.type == ARCHER) {
		AttackOfArcher(player);
	}
}

void Enemy::LoadComponent(Player player , int  map[MAP_HEIGHT][MAP_WIDTH]) {

	if (enemy.type == SLIME) {
		LoadComponentOfSlime(player , map);
	}

	else if (enemy.type == FENCER) {
		LoadComponentOfFencer(player , map);
	}

	else if (enemy.type == ARCHER) {
		LoadComponentOfArcher(player , map);
	}

}

void Enemy::Draw() {


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

void Enemy::NomalMoveOfSlime() {


	if (enemy.isFacingRight == true) {
		enemy.speed.tmpX = enemy.speed.fixedValue;
	}
	else {
		enemy.speed.tmpX = -enemy.speed.fixedValue;
	}

}

void Enemy::AttackOfSlime() {

	if (enemy.attack.motionPhase == 0) {
		enemy.speed.initialspeedY = -3;
		enemy.attack.motionTimer = 25;
		enemy.attack.motionPhase++;
	}

	else if (enemy.attack.motionPhase == 1) {
		enemy.attack.motionTimer--;
		if (enemy.attack.motionTimer <= 0) {
			enemy.attack.motionPhase++;
			enemy.aerialController.isInTheAir = true;
			enemy.aerialController.timer = 0;
		}
	}

	else if (enemy.attack.motionPhase == 2) {

		if (enemy.isFacingRight == true) {
			enemy.speed.tmpX = enemy.speed.fixedValue * 5;
		}
		else {
			enemy.speed.tmpX = -enemy.speed.fixedValue * 5;
		}

		if (enemy.aerialController.isInTheAir == false) {
			enemy.attack.motionPhase++;
			enemy.attack.motionTimer = 30;
		}

	}

	else if (enemy.attack.motionPhase == 3) {
		enemy.attack.motionTimer--;
		if (enemy.attack.motionTimer <= 0) {
			enemy.attack.isAttack = false;
			enemy.attack.motionPhase = 0;
			enemy.attack.coolTime = 5;
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

		DrawBox(enemy.screen.x - enemy.transform.r / 2 ,
				enemy.screen.y + enemy.transform.r ,
				enemy.screen.x ,
				enemy.screen.y - enemy.transform.r ,
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

void Enemy::LoadComponentOfSlime(Player player , int  map[MAP_HEIGHT][MAP_WIDTH]) {
	Component::Enemy::Serch::TrackWithEyes(enemy.eye , enemy.transform , player , map);
	Component::Enemy::Attack::PredictMovement(0 , enemy.target , player);
	Component::Enemy::Attack::InversionAttackAngleBasedFace(enemy.attack.angle , enemy.isFacingRight);
}

#pragma endregion//スライム

#pragma region//フェンサー

void Enemy::NomalMoveOfFencer() {

	if (enemy.isFacingRight == true) {
		enemy.speed.tmpX = enemy.speed.fixedValue;
	}
	else {
		enemy.speed.tmpX = -enemy.speed.fixedValue;
	}

	if (enemy.isFacingRight == true) {
		enemy.attack.weaponAngle = PI / 6;
	}
	else {
		enemy.attack.weaponAngle = PI * 5 / 6;
	}
	weapon->SetAngle(enemy.attack.weaponAngle);
}

void Enemy::AttackOfFencer(Player player) {
	if (enemy.attack.motionPhase == 0) {
		if (enemy.distanceToPlayer < weapon->GetLength() + enemy.transform.r + player.GetTransform().r) {
			enemy.attack.motionPhase = 4;
		}
		else {
			enemy.attack.motionPhase = 1;
		}
	}
#pragma region//攻撃パターン1
	else if (enemy.attack.motionPhase == 1) {
		enemy.speed.tmpX = (enemy.speed.fixedValue + 1) - (0.04 * (float)enemy.attack.motionTimer);

		if (0 < enemy.speed.tmpX) {
			if (enemy.isFacingRight == false) {
				enemy.speed.tmpX *= -1;
				enemy.attack.weaponAngle += (PI * 4 / 6) / 75;
			}
			else {
				enemy.attack.weaponAngle -= (PI * 4 / 6) / 75;
			}
		}

		else {
			enemy.attack.motionPhase++;
			enemy.attack.motionTimer = 0;
		}

		enemy.attack.motionTimer++;
	}

	else if (enemy.attack.motionPhase == 2) {
		if (enemy.attack.motionTimer <= 4) {
			if (enemy.isFacingRight == true) {
				enemy.attack.weaponAngle += (PI / 6);
			}
			else {
				enemy.attack.weaponAngle -= (PI / 6);
			}
			enemy.attack.motionTimer++;
		}
		else {
			enemy.attack.motionTimer = 40;
			enemy.attack.motionPhase++;
		}
	}
	else if (enemy.attack.motionPhase == 3) {
		if (0 <= enemy.attack.motionTimer) {
			enemy.attack.motionTimer--;
		}
		else {
			enemy.attack.motionPhase = 0;
			enemy.attack.motionTimer = 0;
			enemy.attack.isAttack = false;
			enemy.attack.coolTime = 5;
		}
	}
#pragma endregion//攻撃パターン1

#pragma region//攻撃パターン2

	else if (enemy.attack.motionPhase == 4) {
		if (enemy.attack.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.attack.weaponAngle -= (PI / 12);
			}
			else {
				enemy.attack.weaponAngle += (PI / 12);
			}
			enemy.attack.motionTimer++;
		}
		else {
			enemy.attack.motionTimer = 0;
			enemy.attack.motionPhase++;
		}
	}

	else if (enemy.attack.motionPhase == 5) {
		if (enemy.attack.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.attack.weaponAngle += (PI / 12);
			}
			else {
				enemy.attack.weaponAngle -= (PI / 12);
			}
			enemy.attack.motionTimer++;
		}
		else {
			enemy.attack.motionTimer = 0;
			enemy.attack.motionPhase++;
		}
	}

	else if (enemy.attack.motionPhase == 6) {
		if (enemy.attack.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.attack.weaponAngle -= (PI / 12);
			}
			else {
				enemy.attack.weaponAngle += (PI / 12);
			}
			enemy.attack.motionTimer++;
		}
		else {
			enemy.attack.motionTimer = 0;
			enemy.attack.motionPhase++;
		}
	}

	else if (enemy.attack.motionPhase == 7) {
		if (enemy.attack.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.attack.weaponAngle += (PI / 12);
			}
			else {
				enemy.attack.weaponAngle -= (PI / 12);
			}
			enemy.attack.motionTimer++;
		}
		else {
			enemy.attack.motionTimer = 0;
			enemy.attack.motionPhase++;
		}
	}

	else if (enemy.attack.motionPhase == 8) {
		if (enemy.attack.motionTimer <= 6) {
			if (enemy.isFacingRight == true) {
				enemy.attack.weaponAngle -= (PI / 12);
			}
			else {
				enemy.attack.weaponAngle += (PI / 12);
			}
			enemy.attack.motionTimer++;
		}
		else {
			enemy.attack.motionTimer = 50;
			enemy.attack.motionPhase++;
		}
	}

	else if (enemy.attack.motionPhase == 9) {
		if (0 <= enemy.attack.motionTimer) {
			enemy.attack.motionTimer--;
			if (enemy.attack.motionTimer <= 30) {
				if (enemy.isFacingRight == true) {
					enemy.attack.weaponAngle += (PI * 6 / 12) / 28;
				}
				else {
					enemy.attack.weaponAngle -= (PI * 6 / 12) / 28;
				}
			}
		}
		else {
			enemy.attack.motionPhase = 0;
			enemy.attack.motionTimer = 0;
			enemy.attack.isAttack = false;
			enemy.attack.coolTime = 5;
		}
	}

	weapon->SetAngle(enemy.attack.weaponAngle);

#pragma endregion//攻撃パターン2

}

void Enemy::DrawOfFencer() {

	DrawCircle(enemy.screen.x ,
			   enemy.screen.y ,
			   enemy.transform.r ,
			   GetColor(200 , 50 , 50) ,
			   true);

}

void Enemy::LoadComponentOfFencer(Player player , int  map[MAP_HEIGHT][MAP_WIDTH]) {

	Component::Enemy::Serch::TrackWithEyes(enemy.eye , enemy.transform , player , map);
	Component::Enemy::Attack::PredictMovement(0 , enemy.target , player);
	Component::Enemy::Attack::InversionAttackAngleBasedFace(enemy.attack.angle , enemy.isFacingRight);

}

#pragma endregion//フェンサー

#pragma region//アーチャー

void Enemy::NomalMoveOfArcher(Player player) {

	if (enemy.isFacingRight == true) {
		enemy.attack.weaponAngle = 0;
	}
	else {
		enemy.attack.weaponAngle = PI;
	}
}

void Enemy::AttackOfArcher(Player player) {

	if (enemy.attack.motionPhase == 0) {
		enemy.attack.motionTimer = 50;
		enemy.attack.motionPhase++;
	}

	else if (enemy.attack.motionPhase == 1) {

		enemy.attack.weaponAngle = atan2((float)enemy.target.y - enemy.transform.pos.y ,
										 (float)enemy.target.x - enemy.transform.pos.x);

		weapon->SetAngle(enemy.attack.weaponAngle);

		if (0 < enemy.attack.motionTimer) {
			enemy.attack.motionTimer--;
		}
		else {
			enemy.attack.motionPhase++;
			enemy.attack.motionTimer = 200;
			enemy.attack.isShot = true;
			weapon->SetIsShot(enemy.attack.isShot);
		}
	}

	else if (enemy.attack.motionPhase == 2) {

		if (enemy.attack.isShot == true) {
			if (0 < enemy.attack.motionTimer) {
				enemy.attack.motionTimer--;
			}
			else {
				enemy.attack.isShot = false;
				weapon->SetIsShot(enemy.attack.isShot);
			}
		}
		else if (enemy.attack.isShot == false) {
			enemy.attack.motionPhase = 0;
			enemy.attack.coolTime = 100;
			enemy.attack.isAttack = false;
		}

	}

}

void Enemy::DrawOfArcher() {

	DrawCircle(enemy.screen.x ,
			   enemy.screen.y ,
			   enemy.transform.r ,
			   GetColor(200 , 50 , 50) ,
			   true);

	DrawTriangle(enemy.screen.x + (float)cos(enemy.attack.weaponAngle) * enemy.transform.r * 2 ,
				 enemy.screen.y + (float)sin(enemy.attack.weaponAngle) * enemy.transform.r * 2 ,
				 enemy.screen.x + (float)cos(enemy.attack.weaponAngle) * enemy.transform.r + (float)cos(enemy.attack.weaponAngle + PI / 2) * enemy.transform.r ,
				 enemy.screen.y + (float)sin(enemy.attack.weaponAngle) * enemy.transform.r + (float)sin(enemy.attack.weaponAngle + PI / 2) * enemy.transform.r ,
				 enemy.screen.x + (float)cos(enemy.attack.weaponAngle) * enemy.transform.r - (float)cos(enemy.attack.weaponAngle + PI / 2) * enemy.transform.r ,
				 enemy.screen.y + (float)sin(enemy.attack.weaponAngle) * enemy.transform.r - (float)sin(enemy.attack.weaponAngle + PI / 2) * enemy.transform.r ,
				 GetColor(200 , 50 , 50) ,
				 false);

}

void Enemy::LoadComponentOfArcher(Player player , int  map[MAP_HEIGHT][MAP_WIDTH]) {

	Component::Enemy::Serch::TrackWithEyes(enemy.eye , enemy.transform , player , map);
	Component::Enemy::Attack::PredictMovement(weapon->GetSpeed() , enemy.distanceToPlayer , enemy.target , player);
	Component::Enemy::Attack::SetAttackAngleSameAsEyeAngle(enemy.attack.angle , enemy.eye.angle);

}

#pragma endregion//アーチャー

void Enemy::DebugDraw(Scroll scroll) {
	SetDrawBlendMode(DX_BLENDMODE_ALPHA , 100);

#pragma region//DetermineIsAttackHit
	Vector2 attackRangeVert1{
		enemy.transform.pos.x + (float)cos(enemy.attack.angle + enemy.attack.amplitude / 2) * enemy.attack.range ,
		enemy.transform.pos.y + (float)sin(enemy.attack.angle + enemy.attack.amplitude / 2) * enemy.attack.range
	};

	Vector2 attackRangeVert2{
		enemy.transform.pos.x + (float)cos(enemy.attack.angle) * enemy.attack.range ,
		enemy.transform.pos.y + (float)sin(enemy.attack.angle) * enemy.attack.range
	};

	Vector2 attackRangeVert3{
		enemy.transform.pos.x + (float)cos(enemy.attack.angle - enemy.attack.amplitude / 2) * enemy.attack.range ,
		enemy.transform.pos.y + (float)sin(enemy.attack.angle - enemy.attack.amplitude / 2) * enemy.attack.range
	};

	DrawTriangle(enemy.transform.pos.x - scroll.x , enemy.transform.pos.y - scroll.y ,
				 attackRangeVert1.x - scroll.x , attackRangeVert1.y - scroll.y ,
				 attackRangeVert2.x - scroll.x , attackRangeVert2.y - scroll.y ,
				 GetColor(255 , 255 , 0) , true);
	DrawTriangle(enemy.transform.pos.x - scroll.x , enemy.transform.pos.y - scroll.y ,
				 attackRangeVert3.x - scroll.x , attackRangeVert3.y - scroll.y ,
				 attackRangeVert2.x - scroll.x , attackRangeVert2.y - scroll.y ,
				 GetColor(255 , 255 , 0) , true);
#pragma endregion

	//target
	DrawCircle(enemy.target.x - scroll.x , enemy.target.y - scroll.y , 20 , GetColor(255 , 255 , 0) , false);

#pragma region//TrackWithEyes
	Vector2 fovCenter{
		enemy.transform.pos.x + (float)cos(enemy.eye.angle) * enemy.eye.range ,
		enemy.transform.pos.y + (float)sin(enemy.eye.angle) * enemy.eye.range
	};

	Vector2 fovUpper{
		enemy.transform.pos.x + (float)cos(enemy.eye.angle + enemy.eye.fov / 2) * enemy.eye.range ,
		enemy.transform.pos.y + (float)sin(enemy.eye.angle + enemy.eye.fov / 2) * enemy.eye.range
	};
	Vector2 fovLower{
		enemy.transform.pos.x + (float)cos(enemy.eye.angle - enemy.eye.fov / 2) * enemy.eye.range ,
		enemy.transform.pos.y + (float)sin(enemy.eye.angle - enemy.eye.fov / 2) * enemy.eye.range
	};

	DrawLine(enemy.eye.x - scroll.x , enemy.eye.y , enemy.transform.pos.x - scroll.x , enemy.transform.pos.y , GetColor(255 , 255 , 255) , true);
	DrawLine(fovUpper.x - scroll.x , fovUpper.y , enemy.transform.pos.x - scroll.x , enemy.transform.pos.y , GetColor(255 , 255 , 255) , true);
	DrawLine(fovCenter.x - scroll.x , fovCenter.y , fovUpper.x - scroll.x , fovUpper.y , GetColor(255 , 255 , 255) , true);
	DrawLine(fovLower.x - scroll.x , fovLower.y , fovCenter.x - scroll.x , fovCenter.y , GetColor(255 , 255 , 255) , true);
	DrawLine(enemy.transform.pos.x - scroll.x , enemy.transform.pos.y , fovLower.x - scroll.x , fovLower.y , GetColor(255 , 255 , 255) , true);
#pragma endregion


	SetDrawBlendMode(DX_BLENDMODE_NOBLEND , 100);
}