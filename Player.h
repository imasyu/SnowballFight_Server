#pragma once
#include "Engine/GameObject.h"

// Forward declarations
class SnowBall;
class SphereCollider;
class Aim;

class Player : public GameObject
{
    bool isPlayer_;          // ���삷��v���C���[���ǂ���
    int hModel_;             // �v���C���[�̃��f���̃n���h��
    int hGroundModel_;       // �n�ʂ̃��f���̃n���h��
    XMFLOAT3 lastPosition_;  // �O�t���[���̈ʒu���L��
    float accumulatedDistance_;  // �ړ������̗ݐ�
    XMFLOAT3 shotDirection_; // ��ʂ̔��˕���
    SphereCollider* pCollision_; // ���̃R���C�_�[
    Aim* pAim_;              // �Ə�
    SnowBall* pSnowBall_;    // ���

public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    // �v���C���[�̏�����
    void InitializeIsPlayer();

    // �v���C���[���ǂ�����Ԃ�
    bool IsPlayer() const { return isPlayer_; }

    // ��ʂ̐ݒ�
    void SetSnowBall(SnowBall* snowBall) { pSnowBall_ = snowBall; }

    // ��ʂ𔭎˂���
    void Shot();

    // �n�ʂƂ̓����蔻����s��
    void RayCastStage();

    // �v���C���[�łȂ��ꍇ�̈ʒu�ݒ�
    void NotPlayerSetPosition(XMFLOAT3 pos);

    // �I�u�W�F�N�g���m���Փ˂����Ƃ��̏���
    void OnCollision(GameObject* pTarget);

    // �v���C���[�̈ʒu���X�V
    void UpdatePlayerPosition(const XMFLOAT3& moveDirection, float speed);

    // ��ʂ̑傫�����X�V����֐�
    void UpdateSnowBallScale(float scaleCoefficient, float maxScale);

    // 2�_�Ԃ̋������v�Z����
    float CalculateDistanceMoved(const XMFLOAT3& currentPosition, const XMFLOAT3& lastPosition);

    // �Ə��̓��͂Ɋ�Â��Ĉړ��������v�Z����
    XMFLOAT3 CalculateMoveInput(Aim* pAim);

    // ���ʂ̍X�V����
    void CommonUpdate();
};
