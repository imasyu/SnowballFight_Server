#pragma once
#include "Engine/GameObject.h"

class Aim : public GameObject
{
    float perspectiveDistance_;     //どのくらい後ろから映すか
    float mouseSensitivity;         //マウス感度
    XMFLOAT3 cameraTarget_;         //カメラの焦点目標
    XMFLOAT3 cameraPosition_;       //カメラの場所目標
    XMFLOAT3 aimDirection_;         //現在の視点に基づいた進行方向ベクトル
    class Player* pPlayer_;

    void CalcAim();
    void CalcMouseMove();
    XMVECTOR CalcDirection(float x, float y);

public:
    Aim(GameObject* parent);
    ~Aim();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void SetPlayer(class Player* p) { pPlayer_ = p; }
    XMFLOAT3 GetAimDirection() { return aimDirection_; }

};

