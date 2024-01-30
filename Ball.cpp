#include "Ball.h"
#include "Engine/Model.h"

Ball::Ball(GameObject* parent)
	: GameObject(parent, "Ball"), hModel_(-1), time_(0)
{
}

Ball::~Ball()
{
}

void Ball::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("yukidaruma.fbx");
	assert(hModel_ >= 0);

}

void Ball::Update()
{
	time_++;
	if (time_ > 100) {
		KillMe();
		return;
	}

	float s = time_ * time_ * 0.001f;
	transform_.scale_ = { s, s, s };

}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

void Ball::Release()
{
}
