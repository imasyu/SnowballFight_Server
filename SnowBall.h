#pragma once
#include "Engine/GameObject.h"

class SphereCollider;
class Player;

class SnowBall : public GameObject
{
	bool isShot_;
	int hModel_;
	int hGroundModel_;
	int frameCount_;
	XMFLOAT3 velocity_;
	float gravity_;
	SphereCollider* pCollision_;
	Player* pPlayer_;
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

	// 位置と変形の更新
	void UpdateTransformation();

	// レイキャスト確認
	void CheckRayCollision();

	// 破壊条件確認
	void CheckDestructionConditions();

	// サイズをセット
	void SetScale(float scale);

	// 速度をセット
	void SetVelocity(XMFLOAT3 velocity);

	// 撃っているかどうか
	void SetIsShot(bool b) { isShot_ = true; }

	// 何かに当たった
	void OnCollision(GameObject* pTarget) override;

	// プレイヤーを取得するメソッド
	Player* GetPlayer() const { return pPlayer_; }

};

