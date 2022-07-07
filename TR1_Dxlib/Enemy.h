#pragma once
#include "Global.h"
#include "Player.h"
#include "Weapon.h"

class Enemy {

public:
	//コンストラクタの宣言
	Enemy(int enemyType);
	Enemy(EnemyStruct enemy);

	//デストラクタの宣言
	~Enemy();

	//メンバ関数
	void Update(Player player , int  map[MAP_HEIGHT][MAP_WIDTH] , Scroll& scroll);
	void Draw();

private:
	//攻撃が当たるか判定する関数
	int DetermineIsAttackHit();

	void NomalMove(Player player);
	void Attack(Player player);
	void LoadComponent(Player player , int  map[MAP_HEIGHT][MAP_WIDTH]);

	//各敵ごとの関数
	void NomalMoveOfSlime();
	void AttackOfSlime();
	void DrawOfSlime();
	void LoadComponentOfSlime(Player player , int  map[MAP_HEIGHT][MAP_WIDTH]);

	void NomalMoveOfFencer();
	void AttackOfFencer(Player player);
	void DrawOfFencer();
	void LoadComponentOfFencer(Player player , int  map[MAP_HEIGHT][MAP_WIDTH]);

	void NomalMoveOfArcher(Player player);
	void DrawOfArcher();
	void AttackOfArcher(Player player);
	void LoadComponentOfArcher(Player player , int  map[MAP_HEIGHT][MAP_WIDTH]);

	//敵関数テンプレート
	//void NomalMoveOf{EnemyName}();
	//void AttackOf{EnemyName}();
	//void DrawOf{EnemyName}();
	//void LoadComponentOf{EnemyName}(Player player , int  map[MAP_HEIGHT][MAP_WIDTH]);

	void DebugDraw(Scroll scroll);

private:
	//メンバ変数
	EnemyStruct enemy;
	Weapon* weapon;

};

