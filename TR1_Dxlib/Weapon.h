#pragma once
#include "Global.h"

class Weapon {
public:
	//�R���X�g���N�^�̐錾
	Weapon(int weaponType);

	//�f�X�g���N�^�̐錾
	~Weapon();

	//�����o�֐�
	void Update(double x , double y , double angle , Scroll scroll , int map[MAP_HEIGHT][MAP_WIDTH]);

	void Draw();

	//�Z�b�^�[,�Q�b�^�[
	void SetIsShot(int isShot);

private:
	//�����o�ϐ�

	Transform tipTransform;
	Transform rootTransform;
	Screen tipScreen;
	Screen rootScreen;
	Speed speed;
	double angle;
	int weaponType;
	int isShot;

};

