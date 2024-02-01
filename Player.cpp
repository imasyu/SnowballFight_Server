#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "Stage.h"
#include "SnowBall.h"
#include "Engine/Text.h"

namespace {
    Text* pText = nullptr;
    constexpr float PLAYER_SPEED = 0.1f;        // プレイヤーの移動速度
    constexpr float SCALE_COEFFICIENT = 0.1f;   // 雪玉スケールの増加率を調整する係数
    constexpr float MAX_SCALE = 2.0f;           // 雪玉の最大サイズ   
}

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"), hModel_(-1), isPlayer_(false), pAim_(nullptr), hGroundModel_(-1), accumulatedDistance_(0), shotDirection_(0,0,0),
    lastPosition_(0,0,0), pSnowBall_(nullptr), pCollision_(nullptr)
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

    pCollision_ = new SphereCollider(XMFLOAT3(0, 0.8, 0), 1.1);
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
    if (isPlayer_) return;

    transform_.position_ = pos;
    CommonUpdate();
}

void Player::CommonUpdate()
{
    lastPosition_ = transform_.position_;
    XMFLOAT3 moveDirection = CalculateMoveInput(pAim_);
    UpdatePlayerPosition(moveDirection, PLAYER_SPEED);

    // 移動距離の更新
    accumulatedDistance_ += CalculateDistanceMoved(transform_.position_, lastPosition_);
    
    // 雪玉の更新
    UpdateSnowBallScale(SCALE_COEFFICIENT, MAX_SCALE);

    RayCastStage();
}

void Player::Update()
{
    if (!isPlayer_) return;

    // 共通部分の更新
    CommonUpdate();

    // ステージとの判定処理
    RayCastStage();

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
    return sqrt(diff.x * diff.x + diff.z * diff.z);
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
    vec.y += newScale * inGround;
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
    //増加率リセット
    accumulatedDistance_ = 0.0f;

    XMVECTOR vMove = { 0.0f, 0.0f, 1.0f, 0.0f };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY); 
    XMStoreFloat3(&shotDirection_, vMove);

    pSnowBall_->SetPosition(transform_.position_);
    pSnowBall_->SetVelocity(shotDirection_);
    pSnowBall_->SetIsShot(true);
    pSnowBall_ = Instantiate<SnowBall>(GetParent());
    pSnowBall_->SetPlayer(this);
}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    if (isPlayer_) 
    {
        pText->Draw(30, 100, transform_.position_.x);
        pText->Draw(30, 140, transform_.position_.z);
    }
}

void Player::Release()
{
}

void Player::RayCastStage()
{
    RayCastData data;
    data.start = transform_.position_;      // レイの発射位置
    data.start.y = 0;                       // 座標0から撃つ
    data.dir = { 0, -1, 0 };                // レイの方向
    Model::RayCast(hGroundModel_, &data);

    // 当たったら、距離分位置を下げる
    if (data.hit) { transform_.position_.y = -data.dist; }
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
            XMFLOAT3 knockbackDirection = { 1.0f, 0.0f, 1.0f }; // ノックバック方向
            // ノックバックの方向と距離を設定
            float knockbackDistance = 5.0f; // ノックバック距離

            // ノックバック処理
            XMVECTOR vKnockback = XMLoadFloat3(&knockbackDirection);
            vKnockback = XMVector3Normalize(vKnockback) * knockbackDistance;
            XMFLOAT3 knockbackOffset;
            XMStoreFloat3(&knockbackOffset, vKnockback);

            // 新しい位置を設定
            XMFLOAT3 newPosition = transform_.position_;
            newPosition.x += knockbackOffset.x;
            newPosition.y += knockbackOffset.y;
            newPosition.z += knockbackOffset.z;

            // プレイヤーの位置を更新
            transform_.position_ = newPosition;
        }
    }
}