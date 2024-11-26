#include "SnowBall.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Other/Stage.h"
#include "../Other/VFXManager.h"

namespace {
	static const float	SPEED = 0.4f;				//�ړ��X�s�[�h
	static const float	SCALE_CHANGE_RATE = 0.02f;	//�P�t���[���ŃT�C�Y���₷�l
	static const float	LOWER_LIMIT_Y = -10.0f;		//�����艺�̍��W�ɂȂ�����KILL
	static const int	ALIVE_TIME = 240;			//LifeTime
}

SnowBall::SnowBall(GameObject* parent)
	: GameObject(parent, "SnowBall"), hModel_(-1), hGroundModel_(-1), velocity_(0.0f, 0.0f, 0.0f),
	isShot_(false), frameCount_(0), gravity_(0.3f), pCollision_(nullptr), pPlayer_(nullptr)
{
}

void SnowBall::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("Model/SnowBall.fbx");
	assert(hModel_ >= 0);

	Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g��T��
	hGroundModel_ = pStage->GetModelHandle();		//���f���ԍ����擾

	pCollision_ = new SphereCollider(XMFLOAT3(0, 0, 0), 0.1f);
	AddCollider(pCollision_);
}

void SnowBall::Update()
{
	if (!isShot_)return;
	frameCount_++;

	UpdateTransformation();
	CheckRayCollision();
	CheckDestructionConditions();

	VFXManager::CreateSnowBallParticle(transform_.position_);
}

void SnowBall::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void SnowBall::Release()
{
}

void SnowBall::UpdateTransformation()
{
	// �傫���̕ω�
	transform_.scale_.x += SCALE_CHANGE_RATE;
	transform_.scale_.y += SCALE_CHANGE_RATE;
	transform_.scale_.z += SCALE_CHANGE_RATE;

	// �R���W�����̑傫�������f���ɍ��킹��
	pCollision_->SetRadius(transform_.scale_.x);

	// �ړ����x
	transform_.position_.x += velocity_.x;
	transform_.position_.y += velocity_.y;
	transform_.position_.z += velocity_.z;
}

void SnowBall::CheckRayCollision()
{
	RayCastData data;
	data.start = transform_.position_;		// ���C�̔��ˈʒu
	data.start.y = 0;						// ���W0���猂��
	data.dir = XMFLOAT3(0, -1, 0);			// ���C�̕���
	Model::RayCast(hGroundModel_, &data);	// ���C�𔭎�

	float inGround = 0.7f;                  // �����n�ʂɖ��߂�
	// ���C������������
	if (data.hit)
	{
		// ���̕��ʒu��������
		transform_.position_.y = -data.dist;
		transform_.position_.y += transform_.scale_.x * inGround;
	}
	else
	{
		transform_.position_.y -= gravity_;
	}
}

void SnowBall::CheckDestructionConditions()
{
	// �����l���߂��������
	if (transform_.position_.y <= LOWER_LIMIT_Y) KillMe();

	// ��莞�Ԃŏ���
	if (frameCount_ >= ALIVE_TIME) KillMe();

}

void SnowBall::SetScale(float scale)
{
	transform_.scale_ = { scale, scale, scale };
}

void SnowBall::SetVelocity(XMFLOAT3 velocity)
{
	velocity_ = { velocity.x * SPEED, velocity.y * SPEED, velocity.z * SPEED };
}

void SnowBall::OnCollision(GameObject* pTarget)
{
	// ��ʂɓ��������Ƃ�
	if (pTarget->GetObjectName() == "SnowBall")
	{
	}
}