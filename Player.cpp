#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Aim.h"
#include "Stage.h"
#include "SnowBall.h"
#include "Engine/Text.h"

namespace {
    Text* pText = nullptr;
    constexpr float PLAYER_SPEED = 0.1f;        // �v���C���[�̈ړ����x
    constexpr float SCALE_COEFFICIENT = 0.1f;   // ��ʃX�P�[���̑������𒲐�����W��
    constexpr float MAX_SCALE = 2.0f;           // ��ʂ̍ő�T�C�Y   
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
    if (isPlayer_) return;

    transform_.position_ = pos;
    CommonUpdate();
}

void Player::CommonUpdate()
{
    lastPosition_ = transform_.position_;
    XMFLOAT3 moveDirection = CalculateMoveInput(pAim_);
    UpdatePlayerPosition(moveDirection, PLAYER_SPEED);

    // ��ʂɓ������Ă��Ȃ��ꍇ�ɂ݈̂ړ��������X�V
    if (!isSnowHit_)
    {
        accumulatedDistance_ += CalculateDistanceMoved(transform_.position_, lastPosition_);
    }
    
    // ��ʂ̍X�V
    UpdateSnowBallScale(SCALE_COEFFICIENT, MAX_SCALE);

    RayCastStage();
}

void Player::Update()
{
    if (!isPlayer_) return;

    // ���ʕ����̍X�V
    CommonUpdate();

    // �X�e�[�W�Ƃ̔��菈��
    RayCastStage();

    // ��ʂ𔭎˂���
    if (Input::IsKeyDown(DIK_SPACE))
    {
        Shot();
    }

    ///////////////////////////////kokkara
    float delta = 0.3f;
    if (isSnowHit_)
    {
        // ������ւ̈ړ�����
        transform_.position_.y += velocity_.y * delta;

        // �v���C���[�̈ʒu���X�V
        transform_.position_.x += velocity_.x * delta;
        transform_.position_.z += velocity_.z * delta;

        // �m�b�N�o�b�N���Ԃ̍X�V
        knockbackTimer_ -= delta;

        // �m�b�N�o�b�N���I��������t���O�����Z�b�g
        if (knockbackTimer_ <= 0.0f) {
            isSnowHit_ = false;
            velocity_ = XMFLOAT3(0.0f, 0.0f, 0.0f); // �m�b�N�o�b�N�I�����ɑ��x�����Z�b�g
        }
    }
    else
    {
        // �m�b�N�o�b�N���łȂ��ꍇ�ɑ؋󒆂��ǂ����𔻒�
        if (IsInAir())
        {
            // �؋󒆂͒n�ʂɌ������Ĉʒu�����X�ɕ␳
            float correctionSpeed = 0.5f; // �␳���x�𒲐�
            transform_.position_.y -= correctionSpeed * delta;
        }
        else
        {
            // �n�ʂɒ��n���Ă���ꍇ�͑؋��Ԃ����Z�b�g
            float groundHeight = 0.0f;
            if (RayCastGround(groundHeight) && fabs(groundHeight - transform_.position_.y) < 0.0f) // �������C�����ꂽ����
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
    //���������Z�b�g
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
    data.start = transform_.position_;      // ���C�̔��ˈʒu
    data.start.y = 0;                       // ���W0���猂��
    data.dir = { 0, -1, 0 };                // ���C�̕���
    Model::RayCast(hGroundModel_, &data);

    // ����������A�������ʒu��������
    if (data.hit) { transform_.position_.y = -data.dist; }
}

void Player::OnCollision(GameObject* pTarget)
{
    // ��ʂɓ��������Ƃ�
    if (pTarget->GetObjectName() == "SnowBall")
    {
        SnowBall* ball = static_cast<SnowBall*>(pTarget);
        // �v���C���[���g����������ʂłȂ�����ʃq�b�g�t���O���I�t�̂Ƃ�
        /*if (ball->GetPlayer() != this)
        {
            // �m�b�N�o�b�N�̕�����ݒ�(��ʂ̈ʒu-�v���C���[�̈ʒu)
            XMFLOAT3 knockbackDirection = { 1.0f, 0.0f, 1.0f };
            XMVECTOR vKnockbackDirection = XMVectorSubtract(XMLoadFloat3(&ball->GetPosition()), XMLoadFloat3(&transform_.position_));

            // �m�b�N�o�b�N�̈З͂�ݒ�(��ŗ݌v�ړ������ɂ���)
            float KnockbackPower = 2.0f;

            // ����*�З�
            XMVector3Normalize(vKnockbackDirection) * KnockbackPower;

            // float3�ɖ߂�
            XMStoreFloat3(&knockbackDirection, vKnockbackDirection);
            XMFLOAT3 knockbackOffset = knockbackDirection;

            // �V�����ʒu��ݒ�
            XMFLOAT3 newPosition = transform_.position_;
            newPosition.x += knockbackOffset.x;
            newPosition.y += knockbackOffset.y;
            newPosition.z += knockbackOffset.z;

            // �v���C���[�̈ʒu���X�V
            transform_.position_ = newPosition;
        }*/


        ////////////////////////////////////kokkara
        if (ball->GetPlayer() != this && !isSnowHit_)
        {
            // �q�b�g�t���O���Ă�
            isSnowHit_ = true;

            // �m�b�N�o�b�N��
            float knockbackPower = 1.5f;
            
            // �m�b�N�o�b�N�̕�����ݒ�(��ʂ̈ʒu-�v���C���[�̈ʒu)
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
// �V�������\�b�h��ǉ����đ؋󒆂��ǂ����𔻒�
bool Player::IsInAir()
{
    float groundHeight = 0.0f;
    return !RayCastGround(groundHeight) || (transform_.position_.y - groundHeight) < 0;
}

bool Player::RayCastGround(float& groundHeight)
{
    RayCastData data;
    data.start = transform_.position_;      // ���C�̔��ˈʒu
    data.start.y += 1.0f;                    // �v���C���[�̍������烌�C�𔭎�
    data.dir = { 0, -1, 0 };                // ���C�̕���
    Model::RayCast(hGroundModel_, &data);

    if (data.hit) {
        groundHeight = data.dist;
        return true;
    }

    return false;
}

//////////////////////////////////////kokomade