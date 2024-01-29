#include "Stage.h"
#include "Engine/Model.h"

Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), hModel_{-1,-1}
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	// �X�J�C�{�b�N�X�ǂݍ���
	hModel_[0] = Model::Load("SnowBallFight_SkyBox.fbx");
	assert(hModel_[0] >= 0);

	// �n�ʓǂݍ���
	hModel_[1] = Model::Load("iceGround.fbx");
	assert(hModel_[1] >= 0);
}

void Stage::Update()
{
}

void Stage::Draw()
{
	for (int i = 0; i < 2; i++)
	{
		Model::SetTransform(hModel_[i], transform_);
		Model::Draw(hModel_[i]);
	}
}

void Stage::Release()
{
}
