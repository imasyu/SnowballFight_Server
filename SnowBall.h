#pragma once
#include "Engine/GameObject.h"

class SphereCollider;
class Player;

class SnowBall : public GameObject
{
	bool isShot_;
	int hModel_;
	int hGroundModel_;
	int frameCount_;
	XMFLOAT3 velocity_;
	float gravity_;
	SphereCollider* pCollision_;
	Player* pPlayer_;
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

	// �ʒu�ƕό`�̍X�V
	void UpdateTransformation();

	// ���C�L���X�g�m�F
	void CheckRayCollision();

	// �j������m�F
	void CheckDestructionConditions();

	// �T�C�Y���Z�b�g
	void SetScale(float scale);

	// ���x���Z�b�g
	void SetVelocity(XMFLOAT3 velocity);

	// �����Ă��邩�ǂ���
	void SetIsShot(bool b) { isShot_ = true; }

	// �����ɓ�������
	void OnCollision(GameObject* pTarget) override;

	// �v���C���[���擾���郁�\�b�h
	Player* GetPlayer() const { return pPlayer_; }

};

