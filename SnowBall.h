#pragma once
#include "Engine/GameObject.h"
class SnowBall : public GameObject
{
	int hModel_;
	int hGroundModel_;
	XMFLOAT3 velocity_;
public:
	//コンストラクタ
	//引数：parent  親オブジェクト（SceneManager）
	SnowBall(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void SetScale(float scale);

	void SetVelocity(XMFLOAT3 velocity);

};

