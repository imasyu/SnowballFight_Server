#include "SnowBall.h"
#include "Engine/Model.h"

SnowBall::SnowBall(GameObject* parent)
	: GameObject(parent, "SnowBall"), hModel_(-1)
{
}

void SnowBall::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("SnowBall.fbx");
	assert(hModel_ >= 0);

	transform_.position_ = { 0, 1, 1.5f };
}

void SnowBall::Update()
{
	
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
