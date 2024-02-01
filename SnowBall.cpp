#include "SnowBall.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Stage.h"
#include "HitVFX.h"

namespace {
	static const float SPEED = 0.5f;
}

SnowBall::SnowBall(GameObject* parent)
	: GameObject(parent, "SnowBall"), hModel_(-1), hGroundModel_(-1), velocity_(0,0,0), isShot_(false)
{
}

void SnowBall::Initialize()
{
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("SnowBall.fbx");
	assert(hModel_ >= 0);

	Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g��T��
	hGroundModel_ = pStage->GetModelHandle();    //���f���ԍ����擾

}

void SnowBall::Update()
{
	if (isShot_) {
		transform_.scale_.x += 0.03f;
		transform_.scale_.y += 0.03f;
		transform_.scale_.z += 0.03f;

		transform_.position_.x += velocity_.x;
		transform_.position_.y += velocity_.y;
		transform_.position_.z += velocity_.z;

		Rollparticle();

		RayCastData data;
		data.start = transform_.position_;   //���C�̔��ˈʒu
		data.start.y = 0;
		data.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
		Model::RayCast(hGroundModel_, &data); //���C�𔭎�

		//���C������������
		if (data.hit)
		{
			//���̕��ʒu��������
			transform_.position_.y = -data.dist;
			transform_.position_.y += transform_.scale_.x * 0.7f;

			
		}
		else {
			transform_.position_.y -= 0.05f;
		}

		if (transform_.position_.y <= -10.0f) KillMe();
	}

}

void SnowBall::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void SnowBall::Release()
{
}

void SnowBall::SetScale(float scale)
{
	transform_.scale_ = { scale,scale,scale };

}

void SnowBall::SetVelocity(XMFLOAT3 velocity)
{
	velocity_ = { velocity.x * SPEED, velocity.y * SPEED, velocity.z * SPEED };
}

void SnowBall::Rollparticle()
{
	EmitterData data;
	data.textureFileName = "circle_Y.png";
	data.position = XMFLOAT3(transform_.position_.x, transform_.position_.y, transform_.position_.z);
	data.delay = 0;
	data.number = 5;
	data.lifeTime = 60;
	data.positionRnd = XMFLOAT3(0.5, 0, 0.5);
	data.direction = XMFLOAT3(0, 0, 1);
	data.directionRnd = XMFLOAT3(90, 90, 90);
	data.speed = 0.3f;
	data.speedRnd = 0;
	data.accel = 0.93;
	data.size = XMFLOAT2(0.1, 0.1);
	data.sizeRnd = XMFLOAT2(0.4, 0.4);
	data.color = XMFLOAT4(0, 0, 1, 1);
	data.deltaColor = XMFLOAT4(0, 1, 1, 0);
	VFX::Start(data);

	float hitX = transform_.position_.x;
	float hitY = transform_.position_.y;
	float hitZ = transform_.position_.z;

	HitVFX hitVFX;
	hitVFX.CreatePosition(hitX, hitY, hitZ);
}
