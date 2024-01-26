#pragma once
#include "Engine/GameObject.h"	

class Player : public GameObject
{
    bool isPlayer_;     //操作するプレイヤーかどうか
    int hModel_;
    class Aim* pAim_;

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

};

