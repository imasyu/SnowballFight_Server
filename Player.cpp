#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "Stage.h"
#include "SnowBall.h"
#include "Engine/Text.h"

namespace {
    Text* pText = nullptr;

}

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"), hModel_(-1), isPlayer_(false), pAim_(nullptr)
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

    SnowBall* tempSnowBall = Instantiate<SnowBall>(this);
    if (tempSnowBall) {
        tempSnowBall->Initialize();
        SetSnowBall(tempSnowBall);  // pSnowBall_にSnowBallのインスタンスを設定
    }
}

void Player::Update()
{
    lastPosition_ = transform_.position_;

	//操作キャラじゃないなら動かしてみる
	if (!isPlayer_) return;
	
    XMFLOAT3 fMove = { 0,0,0 };
    XMFLOAT3 aimDirection = pAim_->GetAimDirection();
    if (Input::IsKey(DIK_W)) {
        fMove.x += aimDirection.x;
        fMove.z += aimDirection.z;
    }
    if (Input::IsKey(DIK_A)) {
        fMove.x -= aimDirection.z;
        fMove.z += aimDirection.x;
    }
    if (Input::IsKey(DIK_S)) {
        fMove.x -= aimDirection.x;
        fMove.z -= aimDirection.z;
    }
    if (Input::IsKey(DIK_D)) {
        fMove.x += aimDirection.z;
        fMove.z -= aimDirection.x;
    }

    XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
    XMVECTOR vMove = XMLoadFloat3(&fMove);
    vMove = XMVector3Normalize(vMove);
    vMove *= 0.1f;
    XMStoreFloat3(&transform_.position_, vPos + vMove);

    XMFLOAT3 diff;
    diff.x = transform_.position_.x - lastPosition_.x;
    diff.y = transform_.position_.y - lastPosition_.y;
    diff.z = transform_.position_.z - lastPosition_.z;
    float distanceMoved = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    accumulatedDistance_ += distanceMoved;

    //スケールの上限
    if (pSnowBall_ && isPlayer_) {
        float scaleCoefficient = 0.03f; // スケールの増加率を調整する係数
        float maxScale = 2.0;
        float newScale = 0.1f + scaleCoefficient * accumulatedDistance_;
        
        //スケールが最大値を超えないように
        if (newScale > maxScale)
        {
            newScale = maxScale;
        }
        pSnowBall_->SetScale(newScale);
    }

    if (Input::IsKeyDown(DIK_SPACE)) {
        XMVECTOR vAim = XMLoadFloat3(&aimDirection);
        XMVECTOR vTrans = XMLoadFloat3(&transform_.rotate_);

        XMVECTOR vMove = vAim - vTrans;
        vAim = XMVector3Normalize(vAim);
        vAim *= 0.1f;
        XMFLOAT3 move;
        XMStoreFloat3(&move, vAim);

        pSnowBall_ = Instantiate<SnowBall>(GetParent()->GetParent());
        pSnowBall_->SetPosition(transform_.position_);
        pSnowBall_->SetVelocity(aimDirection);
    }

    Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクトを探す
    int hGroundModel = pStage->GetModelHandle();    //モデル番号を取得

    RayCastData data;
    data.start = transform_.position_;   //レイの発射位置
    data.start.y = 0;
    data.dir = XMFLOAT3(0, -1, 0);       //レイの方向
    Model::RayCast(hGroundModel, &data); //レイを発射

    //レイが当たったら
    if (data.hit)
    {
        //その分位置を下げる
        transform_.position_.y = -data.dist;
    }
   
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

    if (isPlayer_) {
        pText->Draw(30, 100, transform_.position_.x);
        pText->Draw(30, 140, transform_.position_.z);
    }
    
}

void Player::Release()
{
}

void Player::InitializeIsPlayer()
{
	isPlayer_ = true;
	pAim_ = Instantiate<Aim>(this);
	pAim_->SetPlayer(this);

}
