#pragma once
#include "Global.h"
#include "Player.h"
#include "Enemy.h"

namespace Component {

	//マップチップとの当たり判定を行う
	void MapchipCollision(Transform& transform , Speed& speed , AerialController& aerialController , int  map[MAP_HEIGHT][MAP_WIDTH]);

	//重力の影響を受ける
	void AffectedByGravity(Speed& speed , AerialController& aerialController);

	namespace Enemy {
		namespace Serch {
			//目で追跡する
			void TrackWithEyes(Eye& eye , Transform enemy , Player player,int  map[MAP_HEIGHT][MAP_WIDTH]);
		}

		namespace Attack{
			//予測射撃
			//飛び道具などのスピードから計算(遠距離武器向け)
			void PredictMovement(Speed speed , float distansToPlayer , Target& target , Player player);
			//攻撃発生までのフレーム数から計算(近接武器向け)
			void PredictMovement(int flame,Target& target , Player player);

			//攻撃の角度を視線の角度と同じにする
			void SetAttackAngleSameAsEyeAngle(float& attackAngle , float eyeAngle);
		
			//攻撃の角度を向きによって反転させる
			void InversionAttackAngleBasedFace(float& attsakAngle,int isFacingRight);
		}
	}
}