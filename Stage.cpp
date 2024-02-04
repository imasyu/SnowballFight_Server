#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"

Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), hModel_{-1,-1}
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	// スカイボックス読み込み
	hModel_[0] = Model::Load("SnowBallFight_SkyBox.fbx");
	assert(hModel_[0] >= 0);

	// 地面読み込み
	hModel_[1] = Model::Load("iceGround.fbx");
	assert(hModel_[1] >= 0);

	transform_.position_ = { 100.0f, 0, 100.0f };
	skyBox_ = transform_;
	skyBox_.scale_ = { 10.0f, 10.0f, 10.0f };

}

void Stage::Update()
{
}

void Stage::Draw()
{
	Direct3D::SetShader(Direct3D::SHADER_SKYBOX);
	Model::SetTransform(hModel_[0], skyBox_);
	Model::Draw(hModel_[0]);
	
	Direct3D::SetShader(Direct3D::SHADER_3D);
	Model::SetTransform(hModel_[1], transform_);
	Model::Draw(hModel_[1]);

}

void Stage::Release()
{
}
