#pragma once
#include "Engine/GameObject.h"	

class SnowBall;

class Player : public GameObject
{
    bool isPlayer_;     //操作するプレイヤーかどうか
    int hModel_;
    int hGroundModel_;

    XMFLOAT3 lastPosition_;  //前フレームの位置を記憶
    float accumulatedDistance_;  // 移動距離の累積

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

};

