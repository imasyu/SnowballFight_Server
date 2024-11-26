#pragma once
#include "../Engine/GameObject.h"

class Player;

class TestScene : public GameObject
{
	Player* pPlayerSelf_;		//���삷��v���C���[�̃|�C���^
	Player* pPlayerOther_;	//���삵�Ȃ��v���C���[�̃|�C���^

public:
	TestScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};