#include "Player.h"
#include "Engine/Model.h"
#include "Aim.h"

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"), hModel_(-1), isPlayer_(false), pAim_(nullptr)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("yukidaruma.fbx");
	assert(hModel_ >= 0);

}

void Player::Update()
{
	if (isPlayer_) {
		transform_.position_.x += 0.01f;

	}

}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

}

void Player::Release()
{
}

void Player::InitializeIsPlayer()
{
	isPlayer_ = true;
	if (isPlayer_) pAim_ = Instantiate<Aim>(this);

}
