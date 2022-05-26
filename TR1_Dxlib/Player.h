#pragma once
#include "Global.h"


class Player {
public:
	//�R���X�g���N�^�̐錾
	Player();
	Player(PlayerStruct player);
	//�f�X�g���N�^�̐錾
	~Player();

	//�����o�֐�
	void Update(char* keys , char* oldkeys , int  map[MAP_HEIGHT][MAP_WIDTH] , Scroll& scroll);
	void Draw();
	double GetPlyaerTransformX();
	double GetPlyaerTransformY();

private:
	//�����o�ϐ�
	PlayerStruct player;
};