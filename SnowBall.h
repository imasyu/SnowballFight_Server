#pragma once
#include "Engine/GameObject.h"
class SnowBall : public GameObject
{
	int hModel_;
	int hGroundModel_;
	XMFLOAT3 velocity_;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	SnowBall(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void SetScale(float scale);

	void SetVelocity(XMFLOAT3 velocity);

};

