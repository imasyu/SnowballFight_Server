#pragma once
#include "Engine/GameObject.h"	

class Player : public GameObject
{
    bool isPlayer_;     //���삷��v���C���[���ǂ���
    int hModel_;
    class Aim* pAim_;

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
    
    //��ʔ���
    void Shot();

};

