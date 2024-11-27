#include "SnowBall.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Other/Stage.h"
#include "../Other/VFXManager.h"

namespace {
	static const float	SPEED = 0.4f;				//移動スピード
	static const float	SCALE_CHANGE_RATE = 0.02f;	//１フレームでサイズ増やす値
	static const float	LOWER_LIMIT_Y = -10.0f;		//これより下の座標になったらKILL
	static const int	ALIVE_TIME = 240;			//LifeTime
}

SnowBall::SnowBall(GameObject* parent)
	: GameObject(parent, "SnowBall"), hModel_(-1), hGroundModel_(-1), velocity_(0.0f, 0.0f, 0.0f),
	isShot_(false), frameCount_(0), gravity_(0.3f), pCollision_(nullptr), pPlayer_(nullptr)
{
}

void SnowBall::Initialize()
{
	//モデルデータのロード
	hModel_ = Model::Load("Model/SnowBall.fbx");
	assert(hModel_ >= 0);

	Stage* pStage = (Stage*)FindObject("Stage");    //ステージオブジェクトを探す
	hGroundModel_ = pStage->GetModelHandle();		//モデル番号を取得

	pCollision_ = new SphereCollider(XMFLOAT3(0, 0, 0), 0.1f);
	AddCollider(pCollision_);
}

void SnowBall::Update()
{
	if (!isShot_)return;
	frameCount_++;

	UpdateTransformation();
	CheckRayCollision();
	CheckDestructionConditions();

	VFXManager::CreateSnowBallParticle(transform_.position_);
}

void SnowBall::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void SnowBall::Release()
{
}

void SnowBall::UpdateTransformation()
{
	// 大きさの変化
	transform_.scale_.x += SCALE_CHANGE_RATE;
	transform_.scale_.y += SCALE_CHANGE_RATE;
	transform_.scale_.z += SCALE_CHANGE_RATE;

	// コリジョンの大きさをモデルに合わせる
	pCollision_->SetRadius(transform_.scale_.x);

	// 移動速度
	transform_.position_.x += velocity_.x;
	transform_.position_.y += velocity_.y;
	transform_.position_.z += velocity_.z;
}

void SnowBall::CheckRayCollision()
{
	RayCastData data;
	data.start = transform_.position_;		// レイの発射位置
	data.start.y = 0;						// 座標0から撃つ
	data.dir = XMFLOAT3(0, -1, 0);			// レイの方向
	Model::RayCast(hGroundModel_, &data);	// レイを発射

	float inGround = 0.7f;                  // 少し地面に埋める
	// レイが当たったら
	if (data.hit)
	{
		// その分位置を下げる
		transform_.position_.y = -data.dist;
		transform_.position_.y += transform_.scale_.x * inGround;
	}
	else
	{
		transform_.position_.y -= gravity_;
	}
}

void SnowBall::CheckDestructionConditions()
{
	// 下限値を過ぎたら消す
	if (transform_.position_.y <= LOWER_LIMIT_Y) KillMe();

	// 一定時間で消す
	if (frameCount_ >= ALIVE_TIME) KillMe();

}

void SnowBall::SetScale(float scale)
{
	transform_.scale_ = { scale, scale, scale };
}

void SnowBall::SetVelocity(XMFLOAT3 velocity)
{
	velocity_ = { velocity.x * SPEED, velocity.y * SPEED, velocity.z * SPEED };
}

void SnowBall::OnCollision(GameObject* pTarget)
{
	// 雪玉に当たったとき
	if (pTarget->GetObjectName() == "SnowBall")
	{
	}
}