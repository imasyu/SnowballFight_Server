#pragma once
#include "../Engine/GameObject.h"

class Stage : public GameObject
{
    int hModel_[2];
    Transform skyBox_;

public:
    Stage(GameObject* parent);
    ~Stage();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    int GetModelHandle() { return hModel_[1]; }
};

