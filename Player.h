#pragma once
#include "Engine/GameObject.h"	

class Player : public GameObject
{
    bool isPlayer_;
    int hModel_;
    class Aim* pAim_;

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    //‘€ì‚·‚éPlayer‚Ì‰Šú‰»
    void InitializeIsPlayer();

};

