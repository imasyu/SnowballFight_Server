#pragma once
#include "Engine/GameObject.h"	

class SnowBall;

class Player : public GameObject
{
    bool isPlayer_;     //���삷��v���C���[���ǂ���
    int hModel_;
    int hGroundModel_;

    XMFLOAT3 lastPosition_;  //�O�t���[���̈ʒu���L��
    float accumulatedDistance_;  // �ړ������̗ݐ�

    class Aim* pAim_;
    class SnowBall* pSnowBall_;
public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //���삷��Player�̏�����
    void InitializeIsPlayer();
    bool IsPlayer() { return isPlayer_; }
    void SetSnowBall(SnowBall* snowBall) { pSnowBall_ = snowBall; }
    void Shot();
    void RayCastStage();
    void NotPlayerSetPosition(XMFLOAT3 pos);

};

