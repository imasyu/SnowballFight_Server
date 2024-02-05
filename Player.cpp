#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "Stage.h"
#include "SnowBall.h"
#include "Engine/Text.h"

namespace {
    Text* pText = nullptr;
    constexpr float PLAYER_SPEED = 0.11f;        // �v���C���[�̈ړ����x
    constexpr float PLAYER_AIR_SPEED = 0.1f;   // �v���C���[�̋󒆂ł̈ړ����x
    constexpr float SCALE_COEFFICIENT = 0.1f;   // ��ʃX�P�[���̑������𒲐�����W��
    constexpr float MAX_SCALE = 2.0f;           // ��ʂ̍ő�T�C�Y   
    constexpr float POWER_XZ = 0.3f;           // ��ʂ̔�΂�����XZ   
    constexpr float POWER_Y = 0.25f;           // ��ʂ̔�΂�����Y
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
	//���f���f�[�^�̃��[�h
	hModel_ = Model::Load("yukidaruma.fbx");
	assert(hModel_ >= 0);

    pText = new Text();
    pText->Initialize();

    //SnowBall�̃C���X�^���X��ݒ�
    pSnowBall_ = Instantiate<SnowBall>(GetParent());
    pSnowBall_->SetPlayer(this);

    Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g��T��
    hGroundModel_ = pStage->GetModelHandle();    //���f���ԍ����擾

    pCollision_ = new SphereCollider(XMFLOAT3(0.0f, 0.8f, 0.0f), 1.1f);
    AddCollider(pCollision_);

    // �}�C�i�X�΍�
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
    // ��ʂɓ������Ă��Ȃ��ꍇ�ɂ݈̂ړ��������X�V
    if (!isSnowHit_) accumulatedDistance_ += CalculateDistanceMoved(transform_.position_, lastPosition_);
 
    // ��ʂ̍X�V
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
        // �ړ�
        if (pAim_) UpdatePlayerPosition(CalculateMoveInput(pAim_), PLAYER_AIR_SPEED);
        
        // �m�b�N�o�b�N�̗}��
        const float knock = 0.015f;
        knockDirection_.x -= knockDirection_.x * knock;
        knockDirection_.z -= knockDirection_.z * knock;

        // �d��
        currentGravity_ -= GRAVITY;

        transform_.position_.x += knockDirection_.x;
        transform_.position_.y += currentGravity_;
        transform_.position_.z += knockDirection_.z;

        // ��ʂ̍X�V
        UpdateSnowBallScale(SCALE_COEFFICIENT, MAX_SCALE);

        // �d�͂��}�C�i�X�̏�ԂȂ�
        if (knockDirection_.y < 0.0f) {
            RayCastData data;
            data.start = transform_.position_;
            data.start.y = 0.0f;
            data.dir = { 0, -1, 0 };
            Model::RayCast(hGroundModel_, &data);

            // ����������A�������ʒu��������
            if (data.hit && -transform_.position_.y > data.dist) {
                transform_.position_.y = -data.dist;
                isSnowHit_ = false;
            }
        }

        return;
    }

    if(!isPlayer_) return;

    // ���s�������璆�S�ɖ߂�
    if (transform_.position_.y <= -30.0f) transform_.position_ = { 100.0f, 0.0f, 100.0f };

    // �ړ�
    lastPosition_ = transform_.position_;
    UpdatePlayerPosition(CalculateMoveInput(pAim_), PLAYER_SPEED);

    // ���ʕ����̍X�V
    CommonUpdate();

    // ��ʂ𔭎˂���
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
    
    float currentScale = pSnowBall_->GetScale().x;  // ���݂̑傫�����擾
    float minScale = 0.1f;                          // �K�����̒l�ɂ���
    float inGround = 0.7f;                          // �����n�ʂɖ��߂�

    // �ړ������Ɋ�Â����V�����傫�����v�Z
    float newScale = minScale + scaleCoefficient * accumulatedDistance_;
    if (newScale > maxScale) { newScale = maxScale; }
    pSnowBall_->SetScale(newScale);

    // ��ʂ̈ʒu���X�V
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
    data.start = transform_.position_;      // ���C�̔��ˈʒu
    data.start.y = 0;                       // ���W0���猂��
    data.dir = { 0, -1, 0 };                // ���C�̕���
    Model::RayCast(hGroundModel_, &data);

    // ����������A�������ʒu��������
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
    // ��ʂɓ��������Ƃ�
    if (pTarget->GetObjectName() == "SnowBall")
    {
        SnowBall* ball = static_cast<SnowBall*>(pTarget);        
        // �v���C���[���g����������ʂłȂ��ꍇ�ɂ̂�
        if (ball->GetPlayer() != this)
        {
            // �m�b�N�o�b�N�̕�����ݒ�(��ʂ̈ʒu-�v���C���[�̈ʒu)
            XMVECTOR vKnockbackDirection = XMVectorSubtract(XMLoadFloat3(&ball->GetPosition()), XMLoadFloat3(&transform_.position_));

            // �m�b�N�o�b�N�̈З͂�ݒ�(��ŗ݌v�ړ������ɂ���)
            float KnockbackPower = ball->GetScale().x;
            KnockbackPower *= POWER_XZ;

            // ����*�З�
            vKnockbackDirection = XMVector3Normalize(vKnockbackDirection) * KnockbackPower;

            // float3�ɖ߂�
            XMStoreFloat3(&knockDirection_, -vKnockbackDirection);
            currentGravity_ = ball->GetScale().x * POWER_Y;

            isSnowHit_ = true;
            ball->KillMe();
        }
        //�G�v���C���[���ł����Ɏ����Ă���ʂ�������
        if (ball->GetIsShot() == false) {
            ball->KillMe();
            ball->GetPlayer()->CreateSnowBall();
        }
    }
}