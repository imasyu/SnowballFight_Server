#pragma once
#include "Engine/GameObject.h"

// Forward declarations
class SnowBall;
class SphereCollider;
class Aim;

class Player : public GameObject
{
    bool isPlayer_;          // 操作するプレイヤーかどうか
    int hModel_;             // プレイヤーのモデルのハンドル
    int hGroundModel_;       // 地面のモデルのハンドル
    XMFLOAT3 lastPosition_;  // 前フレームの位置を記憶
    float accumulatedDistance_;  // 移動距離の累積
    XMFLOAT3 shotDirection_; // 雪玉の発射方向
    SphereCollider* pCollision_; // 球体コライダー
    Aim* pAim_;              // 照準
    SnowBall* pSnowBall_;    // 雪玉

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // プレイヤーの初期化
    void InitializeIsPlayer();

    // プレイヤーかどうかを返す
    bool IsPlayer() const { return isPlayer_; }

    // 雪玉の設定
    void SetSnowBall(SnowBall* snowBall) { pSnowBall_ = snowBall; }

    // 雪玉を発射する
    void Shot();

    // 地面との当たり判定を行う
    void RayCastStage();

    // プレイヤーでない場合の位置設定
    void NotPlayerSetPosition(XMFLOAT3 pos);

    // オブジェクト同士が衝突したときの処理
    void OnCollision(GameObject* pTarget);

    // プレイヤーの位置を更新
    void UpdatePlayerPosition(const XMFLOAT3& moveDirection, float speed);

    // 雪玉の大きさを更新する関数
    void UpdateSnowBallScale(float scaleCoefficient, float maxScale);

    // 2点間の距離を計算する
    float CalculateDistanceMoved(const XMFLOAT3& currentPosition, const XMFLOAT3& lastPosition);

    // 照準の入力に基づいて移動方向を計算する
    XMFLOAT3 CalculateMoveInput(Aim* pAim);

    // 共通の更新処理
    void CommonUpdate();
};
