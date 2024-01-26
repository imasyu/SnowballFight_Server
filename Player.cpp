#include "Player.h"
#include "Engine/Model.h"

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"), hModel_(-1)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/yukidaruma 1.fbx");
	assert(hModel_ >= 0);
	
	transform_.position_ = { 0.0f, 0.0f, 0.0f };

}

void Player::Update()
{
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

void Player::Release()
{
}
