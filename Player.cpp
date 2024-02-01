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
    lastPosition_(0,0,0), pSnowBall_(nullptr), pCollision_(nullptr), isSnowHit_(false), upVelocity_(1.5f)
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
    if (isPlayer_) return;

    transform_.position_ = pos;
    CommonUpdate();
}

void Player::CommonUpdate()
{
    lastPosition_ = transform_.position_;
    XMFLOAT3 moveDirection = CalculateMoveInput(pAim_);
    UpdatePlayerPosition(moveDirection, PLAYER_SPEED);

    // 雪玉に当たっていない場合にのみ移動距離を更新
    if (!isSnowHit_)
    {
        accumulatedDistance_ += CalculateDistanceMoved(transform_.position_, lastPosition_);
    }
    
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

    ///////////////////////////////kokkara
    float delta = 0.3f;
    if (isSnowHit_)
    {
        // 上方向への移動加速
        transform_.position_.y += velocity_.y * delta;

        // プレイヤーの位置を更新
        transform_.position_.x += velocity_.x * delta;
        transform_.position_.z += velocity_.z * delta;

        // ノックバック時間の更新
        knockbackTimer_ -= delta;

        // ノックバックが終了したらフラグをリセット
        if (knockbackTimer_ <= 0.0f) {
            isSnowHit_ = false;
            velocity_ = XMFLOAT3(0.0f, 0.0f, 0.0f); // ノックバック終了時に速度をリセット
        }
    }
    else
    {
        // ノックバック中でない場合に滞空中かどうかを判定
        if (IsInAir())
        {
            // 滞空中は地面に向かって位置を徐々に補正
            float correctionSpeed = 0.5f; // 補正速度を調整
            transform_.position_.y -= correctionSpeed * delta;
        }
        else
        {
            // 地面に着地している場合は滞空状態をリセット
            float groundHeight = 0.0f;
            if (RayCastGround(groundHeight) && fabs(groundHeight - transform_.position_.y) < 0.0f) // ここが修正された部分
            {
                transform_.position_.y = groundHeight;
            }
        }
    }
    /////////////////////////////kokomade
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
        pText->Draw(30, 100, (int)transform_.position_.x);
        pText->Draw(30, 140, (int)transform_.position_.z);
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
        // プレイヤー自身が撃った雪玉でないかつ雪玉ヒットフラグがオフのとき
        /*if (ball->GetPlayer() != this)
        {
            // ノックバックの方向を設定(雪玉の位置-プレイヤーの位置)
            XMFLOAT3 knockbackDirection = { 1.0f, 0.0f, 1.0f };
            XMVECTOR vKnockbackDirection = XMVectorSubtract(XMLoadFloat3(&ball->GetPosition()), XMLoadFloat3(&transform_.position_));

            // ノックバックの威力を設定(後で累計移動距離にする)
            float KnockbackPower = 2.0f;

            // 方向*威力
            XMVector3Normalize(vKnockbackDirection) * KnockbackPower;

            // float3に戻す
            XMStoreFloat3(&knockbackDirection, vKnockbackDirection);
            XMFLOAT3 knockbackOffset = knockbackDirection;

            // 新しい位置を設定
            XMFLOAT3 newPosition = transform_.position_;
            newPosition.x += knockbackOffset.x;
            newPosition.y += knockbackOffset.y;
            newPosition.z += knockbackOffset.z;

            // プレイヤーの位置を更新
            transform_.position_ = newPosition;
        }*/


        ////////////////////////////////////kokkara
        if (ball->GetPlayer() != this && !isSnowHit_)
        {
            // ヒットフラグ立てる
            isSnowHit_ = true;

            // ノックバック力
            float knockbackPower = 1.5f;
            
            // ノックバックの方向を設定(雪玉の位置-プレイヤーの位置)
            XMFLOAT3 knockbackDirection = { 0.0f, 0.0f, 0.0f };
            XMVECTOR vKnockbackDirection = XMVectorSubtract(XMLoadFloat3(&ball->GetPosition()), XMLoadFloat3(&transform_.position_));
            XMVector3Normalize(vKnockbackDirection);
            XMStoreFloat3(&knockbackDirection, vKnockbackDirection);

            velocity_.y = upVelocity_;
            velocity_.x += knockbackDirection.x* knockbackPower;
            velocity_.z += knockbackDirection.z* knockbackPower;
        }

        ////////////////////////////////////////kokomade
    }
}


//////////////////////////////kokkara
// 新しいメソッドを追加して滞空中かどうかを判定
bool Player::IsInAir()
{
    float groundHeight = 0.0f;
    return !RayCastGround(groundHeight) || (transform_.position_.y - groundHeight) < 0;
}

bool Player::RayCastGround(float& groundHeight)
{
    RayCastData data;
    data.start = transform_.position_;      // レイの発射位置
    data.start.y += 1.0f;                    // プレイヤーの高さからレイを発射
    data.dir = { 0, -1, 0 };                // レイの方向
    Model::RayCast(hGroundModel_, &data);

    if (data.hit) {
        groundHeight = data.dist;
        return true;
    }

    return false;
}

//////////////////////////////////////kokomade