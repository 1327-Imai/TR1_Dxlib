#include "Component.h"

//マップチップとの当たり判定を行う
void Component::MapchipCollision(Transform& transform , Speed& speed , AerialController& aerialController , int  map[MAP_HEIGHT][MAP_WIDTH]) {

	MapCollider mapCollider = {0};

	//本来の四隅の縦方向の座標を取得する
	mapCollider.leftTopY = ((int)transform.pos.y - (int)transform.r) / MAP_SIZE;
	mapCollider.leftBottomY = ((int)transform.pos.y + (int)transform.r - 1) / MAP_SIZE;
	mapCollider.rightTopY = ((int)transform.pos.y - (int)transform.r) / MAP_SIZE;
	mapCollider.rightBottomY = ((int)transform.pos.y + (int)transform.r - 1) / MAP_SIZE;

	//本来の四隅の横方向の座標を取得する
	mapCollider.rightTopX = ((int)transform.pos.x + (int)transform.r - 1) / MAP_SIZE;
	mapCollider.rightBottomX = ((int)transform.pos.x + (int)transform.r - 1) / MAP_SIZE;
	mapCollider.leftTopX = ((int)transform.pos.x - (int)transform.r) / MAP_SIZE;
	mapCollider.leftBottomX = ((int)transform.pos.x - (int)transform.r) / MAP_SIZE;

	//空中にいないとき
	if (aerialController.isInTheAir == false) {

		//下に足場がなければ
		if (map[mapCollider.rightBottomY + 1][mapCollider.rightBottomX] == NONE &&
			map[mapCollider.leftBottomY + 1][mapCollider.leftBottomX] == NONE) {
			aerialController.isInTheAir = true;			//空中にいることにする
			speed.initialspeedY = 0;		//Y軸方向の初速度を0にする
		}

	}


	//現在の四隅の縦方向の座標に一回分進んだ場合の座標を取得
	mapCollider.leftTopY = (int)(transform.pos.y + speed.tmpY - transform.r) / MAP_SIZE;
	mapCollider.rightTopY = (int)(transform.pos.y + speed.tmpY - transform.r) / MAP_SIZE;
	mapCollider.leftBottomY = (int)(transform.pos.y + speed.tmpY + transform.r - 1) / MAP_SIZE;
	mapCollider.rightBottomY = (int)(transform.pos.y + speed.tmpY + transform.r - 1) / MAP_SIZE;

	if (speed.tmpY < 0) {
		if (map[mapCollider.rightTopY][mapCollider.rightTopX] == NONE &&
			map[mapCollider.leftTopY][mapCollider.leftTopX] == NONE) {
			speed.y += speed.tmpY;
		}
		else {//天井にあたったら
			aerialController.timer = 0;
			speed.initialspeedY = 0;
		}
	}

	else if (0 <= speed.tmpY) {
		if (map[mapCollider.rightBottomY][mapCollider.rightBottomX] == NONE &&
			map[mapCollider.leftBottomY][mapCollider.leftBottomX] == NONE) {
			speed.y += speed.tmpY;
		}
		else {
			//地面についたら
			aerialController.isInTheAir = false;	//地面にいることにして
			aerialController.timer = 0;			//aerialController.timerを0にして
			transform.pos.y = mapCollider.rightBottomY * MAP_SIZE - transform.r; //押し戻し処理
		}
	}

	//本来の四隅の縦方向の座標を取得する
	mapCollider.leftTopY = ((int)transform.pos.y - (int)transform.r) / MAP_SIZE;
	mapCollider.leftBottomY = ((int)transform.pos.y + (int)transform.r - 1) / MAP_SIZE;
	mapCollider.rightTopY = ((int)transform.pos.y - (int)transform.r) / MAP_SIZE;
	mapCollider.rightBottomY = ((int)transform.pos.y + (int)transform.r - 1) / MAP_SIZE;




	if (transform.pos.x + transform.r < MAP_WIDTH * MAP_SIZE && 0 < transform.pos.x - transform.r) {//指定されたエリア内にいるなら

		if (0 < speed.tmpX) {
			//現在の右上、右下の頂点に一回分右方向に進んだ場合の座標を取得
			mapCollider.rightTopX = (int)(transform.pos.x + speed.tmpX + transform.r - 1) / MAP_SIZE;
			mapCollider.rightBottomX = (int)(transform.pos.x + speed.tmpX + transform.r - 1) / MAP_SIZE;
			//取得した座標の位置のマップチップがNONEであれば横方向のスピードが右に行くように値を入れる
			if (map[mapCollider.rightTopY][mapCollider.rightTopX] == NONE &&
				map[mapCollider.rightBottomY][mapCollider.rightBottomX] == NONE) {
				speed.x += speed.tmpX;
			}
		}
		else {

			//現在の左上、左下の頂点に一回分左方向に進んだ場合の座標を取得
			mapCollider.leftTopX = (transform.pos.x + speed.tmpX - transform.r) / MAP_SIZE;
			mapCollider.leftBottomX = (transform.pos.x + speed.tmpX - transform.r) / MAP_SIZE;
			//取得した座標の位置のマップチップがNONEであれば横方向のスピードが左に行くように値を入れる
			if (map[mapCollider.leftTopY][mapCollider.leftTopX] == NONE &&
				map[mapCollider.leftBottomY][mapCollider.leftBottomX] == NONE) {
				speed.x += speed.tmpX;
			}
		}
	}

	//本来の四隅の横方向の座標を取得する
	mapCollider.rightTopX = (int)((transform.pos.x + transform.r - 1) / MAP_SIZE);
	mapCollider.rightBottomX = (int)((transform.pos.x + transform.r - 1) / MAP_SIZE);
	mapCollider.leftTopX = (int)((transform.pos.x - transform.r) / MAP_SIZE);
	mapCollider.leftBottomX = (int)((transform.pos.x - transform.r) / MAP_SIZE);

}

//重力の影響を受ける
void Component::AffectedByGravity(Speed& speed , AerialController& aerialController) {
	if (aerialController.isInTheAir == true) {

		//落下速度の計算をする
		speed.tmpY = speed.initialspeedY + (G / 50) * aerialController.timer;
		aerialController.timer++;

	}
}

//敵用

//目で追跡する
void Component::Enemy::Serch::TrackWithEyes(Eye& eye , Transform enemy , Player player , int  map[MAP_HEIGHT][MAP_WIDTH]) {

	eye.isVisuable = false;

	eye.angle = atan2(eye.y - enemy.pos.y ,
					  eye.x - enemy.pos.x);

	Vector2 fovUpper{
		enemy.pos.x + (float)cos(eye.angle + eye.fov / 2) * eye.range ,
		enemy.pos.y + (float)sin(eye.angle + eye.fov / 2) * eye.range
	};

	Vector2 fovCenter{
		enemy.pos.x + (float)cos(eye.angle) * eye.range ,
		enemy.pos.y + (float)sin(eye.angle) * eye.range
	};

	Vector2 fovLower{
		enemy.pos.x + (float)cos(eye.angle - eye.fov / 2) * eye.range ,
		enemy.pos.y + (float)sin(eye.angle - eye.fov / 2) * eye.range
	};

	Vector2 crossA1 = enemy.pos - fovUpper;
	Vector2 crossA2 = fovUpper - fovCenter;
	Vector2 crossA3 = fovCenter - fovLower;
	Vector2 crossA4 = fovLower - enemy.pos;

	Vector2 crossB1 = enemy.pos - player.GetTransform().pos;
	Vector2 crossB2 = fovUpper - player.GetTransform().pos;
	Vector2 crossB3 = fovCenter - player.GetTransform().pos;
	Vector2 crossB4 = fovLower - player.GetTransform().pos;


	if (crossA1.cross(crossB1) < 0 &&
		crossA2.cross(crossB2) < 0 &&
		crossA3.cross(crossB3) < 0 &&
		crossA4.cross(crossB4) < 0) {


		Vector2 tmpEye = enemy.pos;
		Vector2 ray = player.GetTransform().pos - enemy.pos;

		ray.nomalize();

		while (true) {
			tmpEye += ray;
			if (map[(int)(tmpEye.y / MAP_SIZE)][(int)(tmpEye.x / MAP_SIZE)] == BLOCK) {
				break;
			}
			else if (tmpEye.x <= player.GetTransform().pos.x + player.GetTransform().r &&
					 player.GetTransform().pos.x - player.GetTransform().r <= tmpEye.x) {
				if (tmpEye.y <= player.GetTransform().pos.y + player.GetTransform().r &&
					player.GetTransform().pos.y - player.GetTransform().r <= tmpEye.y) {
					eye.isVisuable = true;
					eye.x = tmpEye.x;
					eye.y = tmpEye.y;
					break;
				}
			}
		}
	}

}

void  Component::Enemy::Attack::PredictMovement(Speed speed , float distansToPlayer , Target& target , Player player) {

	target.x = player.GetTransform().pos.x;
	target.y = player.GetTransform().pos.y;

	Vector2 playerMoveValue =
	{(player.GetTransform().pos.x - player.GetOldTransform().pos.x) / 5 ,
		(player.GetTransform().pos.y - player.GetOldTransform().pos.y) / 5};

	float time = distansToPlayer / speed.fixedValue;

	target.x += playerMoveValue.x * time;
	target.y += playerMoveValue.y * time;

}

void  Component::Enemy::Attack::PredictMovement(int flame , Target& target , Player player) {

	target.x = player.GetTransform().pos.x;
	target.y = player.GetTransform().pos.y;

	Vector2 playerMoveValue =
	{(player.GetTransform().pos.x - player.GetOldTransform().pos.x) / 5 ,
		(player.GetTransform().pos.y - player.GetOldTransform().pos.y) / 5};


	target.x += playerMoveValue.x * flame;
	target.y += playerMoveValue.y * flame;

}

void  Component::Enemy::Attack::SetAttackAngleSameAsEyeAngle(float& attackAngle , float eyeAngle) {
	attackAngle = eyeAngle;
}

void Component::Enemy::Attack::InversionAttackAngleBasedFace(float& attackAngle , int isFacingRight) {

	if (isFacingRight) {
		attackAngle = 0;
	}
	else {
		attackAngle = PI;
	}

}

