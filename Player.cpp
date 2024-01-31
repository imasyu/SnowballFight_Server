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
	: GameObject(parent, "Player"), hModel_(-1), isPlayer_(false), pAim_(nullptr), hGroundModel_(-1), accumulatedDistance_(0),
    lastPosition_(0,0,0), pSnowBall_(nullptr), pCollision_(nullptr)
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

    Stage* pStage = (Stage*)FindObject("Stage");    //�X�e�[�W�I�u�W�F�N�g��T��
    hGroundModel_ = pStage->GetModelHandle();    //���f���ԍ����擾

    pCollision_ = new SphereCollider(XMFLOAT3(0, 0.8, 0), 1.1);
    AddCollider(pCollision_);

    transform_.position_ = { 100.0f, 0.0f, 100.0f };
    lastPosition_ = transform_.position_;

}

void Player::Update()
{
    //�ړ�
    if (!isPlayer_) return;
        
    lastPosition_ = transform_.position_;

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
    diff.z = transform_.position_.z - lastPosition_.z;
    float distanceMoved = sqrt(diff.x * diff.x + diff.z * diff.z);
    accumulatedDistance_ += distanceMoved;

    //�X�P�[���̏��
    if (pSnowBall_) {
        float scaleCoefficient = 0.1f; // �X�P�[���̑������𒲐�����W��
        float maxScale = 2.0;
        float newScale = 0.1f + scaleCoefficient * accumulatedDistance_;

        //�X�P�[�����ő�l�𒴂��Ȃ��悤��
        if (newScale > maxScale)
        {
            newScale = maxScale;
        }
        pSnowBall_->SetScale(newScale);

        //�|�W�V�����̃Z�b�g
        XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
        XMVECTOR vMove = { 0.0f, 0.0f, 1.0f, 0.0f };
        XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
        vMove = XMVector3TransformCoord(vMove, mRotY);
        vMove += (newScale / maxScale) * vMove;
        XMFLOAT3 vec = XMFLOAT3();
        XMStoreFloat3(&vec, vPos + vMove);
        vec.y += pSnowBall_->GetScale().x * 0.7f;
        pSnowBall_->SetPosition(vec);

    }

    RayCastStage();

    if (isPlayer_ && Input::IsKeyDown(DIK_SPACE)) {
        Shot();
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

void Player::Shot()
{
    //���������Z�b�g
    accumulatedDistance_ = 0.0f;

    XMVECTOR vMove = { 0.0f, 0.0f, 1.0f, 0.0f };
    XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    vMove = XMVector3TransformCoord(vMove, mRotY);
    XMFLOAT3 shotDirection;
    XMStoreFloat3(&shotDirection, vMove);

    pSnowBall_->SetPosition(transform_.position_);
    pSnowBall_->SetVelocity(shotDirection);
    pSnowBall_->SetIsShot(true);
    pSnowBall_ = Instantiate<SnowBall>(GetParent());
}

void Player::RayCastStage()
{
    RayCastData data;
    data.start = transform_.position_;   //���C�̔��ˈʒu
    data.start.y = 0;
    data.dir = XMFLOAT3(0, -1, 0);       //���C�̕���
    Model::RayCast(hGroundModel_, &data); //���C�𔭎�

    //���C������������
    if (data.hit)
    {
        //���̕��ʒu��������
        transform_.position_.y = -data.dist;
    }

}

void Player::NotPlayerSetPosition(XMFLOAT3 pos)
{
    lastPosition_ = transform_.position_;
    transform_.position_ = pos;

    XMFLOAT3 diff;
    diff.x = pos.x - lastPosition_.x;
    diff.z = pos.z - lastPosition_.z;
    float distanceMoved = sqrt(diff.x * diff.x + diff.z * diff.z);
    accumulatedDistance_ += distanceMoved;

    //�X�P�[���̏��
    if (pSnowBall_) {
        float scaleCoefficient = 0.1f; // �X�P�[���̑������𒲐�����W��
        float maxScale = 2.0;
        float newScale = 0.1f + scaleCoefficient * accumulatedDistance_;

        //�X�P�[�����ő�l�𒴂��Ȃ��悤��
        if (newScale > maxScale)
        {
            newScale = maxScale;
        }
        pSnowBall_->SetScale(newScale);

        RayCastStage();

        //�|�W�V�����̃Z�b�g
        XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
        XMVECTOR vMove = { 0.0f, 0.0f, 1.0f, 0.0f };
        XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
        vMove = XMVector3TransformCoord(vMove, mRotY);
        vMove += (newScale / maxScale) * vMove;
        XMFLOAT3 vec = XMFLOAT3();
        XMStoreFloat3(&vec, vPos + vMove);
        vec.y += pSnowBall_->GetScale().x * 0.7f;
        pSnowBall_->SetPosition(vec);
    }
}

void Player::OnCollision(GameObject* pTarget)
{
    // ��ʂɓ��������Ƃ�
    if (pTarget->GetObjectName() == "SnowBall")
    {
        // �v���C���[���g����������ʂłȂ��ꍇ�ɂ̂�
        if (pSnowBall_ && pSnowBall_->GetPlayer() != this)
        {


            XMFLOAT3 knockbackDirection = { 1.0f, 0.0f, 1.0f }; // �m�b�N�o�b�N����
            // �m�b�N�o�b�N�̕����Ƌ�����ݒ�
            float knockbackDistance = 5.0f; // �m�b�N�o�b�N����

            // �m�b�N�o�b�N����
            XMVECTOR vKnockback = XMLoadFloat3(&knockbackDirection);
            vKnockback = XMVector3Normalize(vKnockback) * knockbackDistance;
            XMFLOAT3 knockbackOffset;
            XMStoreFloat3(&knockbackOffset, vKnockback);

            // �V�����ʒu��ݒ�
            XMFLOAT3 newPosition = transform_.position_;
            newPosition.x += knockbackOffset.x;
            newPosition.y += knockbackOffset.y;
            newPosition.z += knockbackOffset.z;

            // �v���C���[�̈ʒu���X�V
            transform_.position_ = newPosition;

            /*
            // ��ʂ̐i�s�����x�N�g�����擾
            XMFLOAT3 snowballDirection = pSnowBall_->GetVelocity();

            // �m�b�N�o�b�N����
            XMVECTOR vKnockback = XMLoadFloat3(&snowballDirection);
            vKnockback = XMVector3Normalize(vKnockback) * knockbackDistance;
            XMFLOAT3 knockbackOffset;
            XMStoreFloat3(&knockbackOffset, vKnockback);

            transform_.position_.x += knockbackOffset.x;
            transform_.position_.y += knockbackOffset.y;
            transform_.position_.z += knockbackOffset.z;
            */

        }
    }
}