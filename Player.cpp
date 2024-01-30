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
    lastPosition_(0,0,0), pSnowBall_(nullptr)
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

    transform_.position_ = { 100.0f, 0.0f, 100.0f };

}

void Player::Update()
{
    lastPosition_ = transform_.position_;

	//����L��������Ȃ��Ȃ瓮�����Ă݂�
    if (!isPlayer_) {
        RayCastStage();
        return;
    };
	
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

    //�X�P�[���̏��
    if (pSnowBall_ && isPlayer_) {
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
        vMove = { 0.0f, 0.0f, 1.0f, 0.0f };
        XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
        vMove = XMVector3TransformCoord(vMove, mRotY);
        vMove += newScale / maxScale * vMove;
        XMFLOAT3 vec = XMFLOAT3();
        XMStoreFloat3(&vec, vPos + vMove);
        pSnowBall_->SetPosition(vec);

    }

    if (Input::IsKeyDown(DIK_SPACE)) {
        Shot();
    }
    
    RayCastStage();
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

    if(isPlayer_) {
        XMFLOAT3 aimDirection = pAim_->GetAimDirection();
        XMVECTOR vAim = XMLoadFloat3(&aimDirection);
        XMVECTOR vTrans = XMLoadFloat3(&transform_.rotate_);

        XMVECTOR vMove = vAim - vTrans;
        vAim = XMVector3Normalize(vAim);
        vAim *= 0.1f;
        XMFLOAT3 move;
        XMStoreFloat3(&move, vAim);
        pSnowBall_->SetPosition(transform_.position_);
        pSnowBall_->SetVelocity(aimDirection);
        pSnowBall_->SetIsShot(true);

        pSnowBall_ = Instantiate<SnowBall>(GetParent());
        
    }
    else {
        //�����Ă�������֑O�i
        XMVECTOR vPos = XMLoadFloat3(&transform_.position_);
        XMVECTOR vMove = { 0.0f, 0.0f, 1.0f, 0.0f };
        XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
        vMove = XMVector3TransformCoord(vMove, mRotY);
        XMFLOAT3 vec = XMFLOAT3();
        XMStoreFloat3(&vec, vMove);
        pSnowBall_->SetPosition(transform_.position_);
        pSnowBall_->SetVelocity(vec);
        pSnowBall_->SetIsShot(true);

        pSnowBall_ = Instantiate<SnowBall>(GetParent());

    }

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
