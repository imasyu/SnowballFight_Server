#include "Aim.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Player.h"

namespace {
    static const float HEIGHT_DISTANCE = 3.0f;
    static const float UP_MOUSE_LIMIT = -60.0f;
    static const float DOWN_MOUSE_LIMIT = 60.0f;
    static const float MOUSE_SPEED = 0.05f;

}

Aim::Aim(GameObject* parent)
    : GameObject(parent, "Aim"), cameraPosition_{ 0,0,0 }, cameraTarget_{ 0,0,0 }, aimDirection_{ 0,0,0 },
    pPlayer_(nullptr), isMove_(true), mouseSensitivity(0.0f), perspectiveDistance_(0.0f)
{
    mouseSensitivity = 2.0f;
    perspectiveDistance_ = 10.0f;

}

Aim::~Aim()
{
}

void Aim::Initialize()
{
    transform_.rotate_.y = 360.0f;

}

void Aim::Update()
{
    if (Input::IsKeyDown(DIK_E)) isMove_ = !isMove_;

    if (isMove_) {
        CalcMouseMove();
        pPlayer_->SetRotateY(transform_.rotate_.y - 180.0f);
    }

    CalcAim();

}

void Aim::Draw()
{
}

void Aim::Release()
{
}

void Aim::CalcAim()
{
    XMVECTOR direction = CalcDirection(transform_.rotate_.x, transform_.rotate_.y);
    XMStoreFloat3(&aimDirection_, -direction);

    //プレイヤーの位置をカメラの焦点とする        
    XMFLOAT3 plaPos = pPlayer_->GetPosition();
    cameraTarget_ = { plaPos.x, plaPos.y + HEIGHT_DISTANCE, plaPos.z };

    XMVECTOR camPos = XMLoadFloat3(&cameraTarget_) + (direction * perspectiveDistance_);
    XMStoreFloat3(&cameraPosition_, camPos);

    //カメラ情報をセット
    Camera::SetPosition(cameraPosition_);
    Camera::SetTarget(cameraTarget_);
}

void Aim::CalcMouseMove()
{
    XMFLOAT3 mouseMove = Input::GetMouseMove(); //マウスの移動量を取得
    transform_.rotate_.y += (mouseMove.x * MOUSE_SPEED) * mouseSensitivity; //横方向の回転
    transform_.rotate_.x -= (mouseMove.y * MOUSE_SPEED) * mouseSensitivity; //縦方向の回転

    //マイナスにならないように抑制
    if (transform_.rotate_.y <= 0.0f) transform_.rotate_.y = 360.0f;

    //抑制
    if (transform_.rotate_.x <= UP_MOUSE_LIMIT) transform_.rotate_.x = UP_MOUSE_LIMIT;
    if (transform_.rotate_.x >= DOWN_MOUSE_LIMIT) transform_.rotate_.x = DOWN_MOUSE_LIMIT;
}

XMVECTOR Aim::CalcDirection(float x, float y)
{
    XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(x));
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(y));
    XMMATRIX mView = mRotX * mRotY;
    const XMVECTOR forwardVector = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    XMVECTOR direction = XMVector3TransformNormal(forwardVector, mView); //XMVector3TransformNormalを使用することで回転のみを適用します
    return XMVector3Normalize(direction);
}