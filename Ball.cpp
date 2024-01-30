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

	transform_.position_.x = 5.0f;
	transform_.position_.z = 5.0f;

}

void Ball::Update()
{
	time_++;
	if (time_ > 100) {
		KillMe();
		return;
	}

	transform_.position_.y += 0.3f;

}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

void Ball::Release()
{
}
