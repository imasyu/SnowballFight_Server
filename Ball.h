#pragma once
#include "Engine/GameObject.h"

class Ball : public GameObject
{
    int hModel_;
    int time_;

public:
    Ball(GameObject* parent);
    ~Ball();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

};

