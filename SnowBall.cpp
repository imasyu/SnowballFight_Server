#include "SnowBall.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Stage.h"

namespace {
	static const float SPEED = 0.5f;
}

SnowBall::SnowBall(GameObject* parent)
	: GameObject(parent, "SnowBall"), hModel_(-1), hGroundModel_(-1)
{
}

void SnowBall::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("SnowBall.fbx");
	assert(hModel_ >= 0);

	transform_.position_ = { 0, 1, 1.5f };

	Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクトを探す
	hGroundModel_ = pStage->GetModelHandle();    //モデル番号を取得

}

void SnowBall::Update()
{
	transform_.position_.x += velocity_.x;
	transform_.position_.y += velocity_.y;
	transform_.position_.z += velocity_.z;


	RayCastData data;
	data.start = transform_.position_;   //レイの発射位置
	data.start.y = 0;
	data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
	Model::RayCast(hGroundModel_, &data); //レイを発射

	//レイが当たったら
	if (data.hit)
	{
		//その分位置を下げる
		transform_.position_.y = -data.dist;
		transform_.position_.y += transform_.scale_.x * 0.7f;
	}
	else {
		KillMe();
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
