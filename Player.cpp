#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "Stage.h"
#include "SnowBall.h"
#include "Engine/Text.h"

namespace {
    Text* pText = nullptr;
    constexpr float PLAYER_SPEED = 0.11f;        // プレイヤーの移動速度
    constexpr float PLAYER_AIR_SPEED = 0.1f;   // プレイヤーの空中での移動速度
    constexpr float SCALE_COEFFICIENT = 0.1f;   // 雪玉スケールの増加率を調整する係数
    constexpr float MAX_SCALE = 2.0f;           // 雪玉の最大サイズ   
    constexpr float POWER_XZ = 0.3f;           // 雪玉の飛ばす強さXZ   
    constexpr float POWER_Y = 0.25f;           // 雪玉の飛ばす強さY
    const float GRAVITY = 0.01f;

}

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"), hModel_(-1), isPlayer_(false), pAim_(nullptr), hGroundModel_(-1), accumulatedDistance_(0),
    lastPosition_(0,0,0), pSnowBall_(nullptr), pCollision_(nullptr), isSnowHit_(false), knockDirection_(0,0,0), currentGravity_(0)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("yukidaruma.fbx");
	assert(hModel_ >= 0);

    pText = new Text();
    pText->Initialize();

    //SnowBallのインスタンスを設定
    pSnowBall_ = Instantiate<SnowBall>(GetParent());
    pSnowBall_->SetPlayer(this);

    Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクトを探す
    hGroundModel_ = pStage->GetModelHandle();    //モデル番号を取得

    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.8f, 0.0f), 1.1f);
    AddCollider(pCollision_);

    // マイナス対策
    transform_.position_ = { 100.0f, 0.0f, 100.0f };
    lastPosition_ = transform_.position_;

}

void Player::InitializeIsPlayer()
{
    isPlayer_ = true;
    pAim_ = Instantiate<Aim>(this);
    pAim_->SetPlayer(this);
}

void Player::NotPlayerSetPosition(XMFLOAT3 pos)
{
    lastPosition_.x = transform_.position_.x;
    lastPosition_.z = transform_.position_.z;

    transform_.position_.x = pos.x;
    transform_.position_.z = pos.z;

    CommonUpdate();

}

void Player::CommonUpdate()
{
    // 雪玉に当たっていない場合にのみ移動距離を更新
    if (!isSnowHit_) accumulatedDistance_ += CalculateDistanceMoved(transform_.position_, lastPosition_);
 
    // 雪玉の更新
    UpdateSnowBallScale(SCALE_COEFFICIENT, MAX_SCALE);

    RayCastStage();
}

void Player::CreateSnowBall()
{
    accumulatedDistance_ = 0.0f;
    pSnowBall_ = Instantiate<SnowBall>(GetParent());
    pSnowBall_->SetPlayer(this);
}

void Player::Update()
{
    if (isSnowHit_) {
        // 移動
        if (pAim_) UpdatePlayerPosition(CalculateMoveInput(pAim_), PLAYER_AIR_SPEED);
        
        // ノックバックの抑制
        const float knock = 0.015f;
        knockDirection_.x -= knockDirection_.x * knock;
        knockDirection_.z -= knockDirection_.z * knock;

        // 重力
        currentGravity_ -= GRAVITY;

        transform_.position_.x += knockDirection_.x;
        transform_.position_.y += currentGravity_;
        transform_.position_.z += knockDirection_.z;

        // 雪玉の更新
        UpdateSnowBallScale(SCALE_COEFFICIENT, MAX_SCALE);

        // 重力がマイナスの状態なら
        if (knockDirection_.y < 0.0f) {
            RayCastData data;
            data.start = transform_.position_;
            data.start.y = 0.0f;
            data.dir = { 0, -1, 0 };
            Model::RayCast(hGroundModel_, &data);

            // 当たったら、距離分位置を下げる
            if (data.hit && -transform_.position_.y > data.dist) {
                transform_.position_.y = -data.dist;
                isSnowHit_ = false;
            }
        }

        return;
    }

    if(!isPlayer_) return;

    // 下行ったから中心に戻す
    if (transform_.position_.y <= -30.0f) transform_.position_ = { 100.0f, 0.0f, 100.0f };

    // 移動
    lastPosition_ = transform_.position_;
    UpdatePlayerPosition(CalculateMoveInput(pAim_), PLAYER_SPEED);

    // 共通部分の更新
    CommonUpdate();

    // 雪玉を発射する
    if (Input::IsKeyDown(DIK_SPACE))
    {
        Shot();
    }
}

float Player::CalculateDistanceMoved(const XMFLOAT3& currentPosition, const XMFLOAT3& lastPosition)
{
    XMFLOAT3 diff{ 0.0f, 0.0f, 0.0f };
    diff.x = currentPosition.x - lastPosition.x;
    diff.z = currentPosition.z - lastPosition.z;
    return sqrt((float)diff.x * (float)diff.x + (float)diff.z * (float)diff.z);
}

void Player::UpdateSnowBallScale(float scaleCoefficient, float maxScale)
{
    if (!pSnowBall_) return;
    
    float currentScale = pSnowBall_->GetScale().x;  // 現在の大きさを取得
    float minScale = 0.1f;                          // 必ず正の値にする
    float inGround = 0.7f;                          // 少し地面に埋める

    // 移動距離に基づいた新しい大きさを計算
    float newScale = minScale + scaleCoefficient * accumulatedDistance_;
    if (newScale > maxScale) { newScale = maxScale; }
    pSnowBall_->SetScale(newScale);

    // 雪玉の位置を更新
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMove = { 0.0f, 0.0f, 1.0f, 0.0f };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    vMove += (newScale / maxScale) * vMove;
    XMFLOAT3 vec = {0.0f, 0.0f, 0.0f};
    XMStoreFloat3(&vec, vPos + vMove);
    vec.y += newScale;
    pSnowBall_->SetPosition(vec);
}

XMFLOAT3 Player::CalculateMoveInput(Aim* pAim)
{
    XMFLOAT3 aimDirection = pAim->GetAimDirection();
    XMFLOAT3 inputDirection = { 0, 0, 0 };

    if (Input::IsKey(DIK_W)) {
        inputDirection.x += aimDirection.x;
        inputDirection.z += aimDirection.z;
    }
    if (Input::IsKey(DIK_A)) {
        inputDirection.x -= aimDirection.z;
        inputDirection.z += aimDirection.x;
    }
    if (Input::IsKey(DIK_S)) {
        inputDirection.x -= aimDirection.x;
        inputDirection.z -= aimDirection.z;
    }
    if (Input::IsKey(DIK_D)) {
        inputDirection.x += aimDirection.z;
        inputDirection.z -= aimDirection.x;
    }
    return inputDirection;
}

void Player::UpdatePlayerPosition(const XMFLOAT3& moveDirection, float speed)
{
    XMVECTOR vMove = XMVector3Normalize(XMLoadFloat3(&moveDirection)) * speed;
    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMStoreFloat3(&transform_.position_, vPos + vMove);
}

void Player::Shot()
{
    XMVECTOR vMove = { 0.0f, 0.0f, 1.0f, 0.0f };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY); 
    XMFLOAT3 direction = XMFLOAT3();
    XMStoreFloat3(&direction, vMove);

    pSnowBall_->SetPosition(transform_.position_);
    pSnowBall_->SetVelocity(direction);
    pSnowBall_->SetIsShot(true);

    CreateSnowBall();

}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    if (isPlayer_) 
    {
        pText->Draw(30, 100, (int)transform_.position_.x);
        pText->Draw(30, 140, (int)transform_.position_.z);
    }
}

void Player::Release()
{
}

bool Player::RayCastStage()
{
    RayCastData data;
    data.start = transform_.position_;      // レイの発射位置
    data.start.y = 0;                       // 座標0から撃つ
    data.dir = { 0, -1, 0 };                // レイの方向
    Model::RayCast(hGroundModel_, &data);

    // 当たったら、距離分位置を下げる
    if (data.hit) { 
        transform_.position_.y = -data.dist; 
        currentGravity_ = 0.0f;
        return true;
    } else {
        currentGravity_ -= GRAVITY;
        transform_.position_.y += currentGravity_;
        return false;
    }

}

void Player::OnCollision(GameObject* pTarget)
{
    // 雪玉に当たったとき
    if (pTarget->GetObjectName() == "SnowBall")
    {
        SnowBall* ball = static_cast<SnowBall*>(pTarget);        
        // プレイヤー自身が撃った雪玉でない場合にのみ
        if (ball->GetPlayer() != this)
        {
            // ノックバックの方向を設定(雪玉の位置-プレイヤーの位置)
            XMVECTOR vKnockbackDirection = XMVectorSubtract(XMLoadFloat3(&ball->GetPosition()), XMLoadFloat3(&transform_.position_));

            // ノックバックの威力を設定(後で累計移動距離にする)
            float KnockbackPower = ball->GetScale().x;
            KnockbackPower *= POWER_XZ;

            // 方向*威力
            vKnockbackDirection = XMVector3Normalize(vKnockbackDirection) * KnockbackPower;

            // float3に戻す
            XMStoreFloat3(&knockDirection_, -vKnockbackDirection);
            currentGravity_ = ball->GetScale().x * POWER_Y;

            isSnowHit_ = true;
            ball->KillMe();
        }
        //敵プレイヤーが打たずに持っている玉だったら
        if (ball->GetIsShot() == false) {
            ball->KillMe();
            ball->GetPlayer()->CreateSnowBall();
        }
    }
}