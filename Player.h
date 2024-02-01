#pragma once
#include "Engine/GameObject.h"	

class SnowBall;
class SphereCollider;

class Player : public GameObject
{
    bool isPlayer_;     //操作するプレイヤーかどうか
    int hModel_;
    int hGroundModel_;

    XMFLOAT3 lastPosition_;  //前フレームの位置を記憶
    float accumulatedDistance_;  // 移動距離の累積
    XMFLOAT3 shotDirection_;
    Player* pPlayer_;
    SphereCollider* pCollision_;
    class Aim* pAim_;
    class SnowBall* pSnowBall_;
public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //操作するPlayerの初期化
    void InitializeIsPlayer();
    bool IsPlayer() { return isPlayer_; }
    void SetSnowBall(SnowBall* snowBall) { pSnowBall_ = snowBall; }
    void Shot();
    void RayCastStage();
    void NotPlayerSetPosition(XMFLOAT3 pos);
    void OnCollision(GameObject* pTarget);

    void UpdatePlayerPosition(const XMFLOAT3& moveDirection, float speed);

    /// <summary>
    /// 雪玉の大きさを更新する関数
    /// </summary>
    /// <param name="scaleCoefficient">スケールの増加率を調整する係数</param>
    /// <param name="maxScale">最大値</param>
    void UpdateSnowBallScale(float scaleCoefficient, float maxScale);

    float CalculateDistanceMoved(const XMFLOAT3& currentPosition, const XMFLOAT3& lastPosition);

    XMFLOAT3 CalculateMoveInput(Aim* pAim);

    void CommonUpdate();
};

