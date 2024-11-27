#pragma once
#include "../Engine/GameObject.h"

class Player;

class TestScene : public GameObject
{
	Player* pPlayerSelf_;		//操作するプレイヤーのポインタ
	Player* pPlayerOther_;	//操作しないプレイヤーのポインタ

public:
	TestScene(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};