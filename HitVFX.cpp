#include "HitVFX.h"

HitVFX::HitVFX()
{
	////�摜�t�@�C����
	//data.textureFileName = "defaultParticle.png";
	////�G�~�b�^�[�̈ʒu
	//data.position = { 0.0f,0.0f,0.0f };
	//data.delay = 0;
	////�p�[�e�B�N���̐F
	//data.color = { 1.0f,1.0f,1.0f,1.0f };
	////�p�[�e�B�N���̕��ƍ���
	//data.size = { 1.0f,1.0f };
	////�p�[�e�B�N�������܂�Ă��������܂ł̃t���[����
	//data.lifeTime = 30;

	data.textureFileName = "cloudA.png";
	data.position = XMFLOAT3(-4, 4, 4);
	data.positionRnd = XMFLOAT3(0.1, 0, 0.1);
	data.delay = 5;
	data.number = 1;
	data.lifeTime = 60;
	data.gravity = -0.002f;
	data.direction = XMFLOAT3(0, 1, 0);
	data.directionRnd = XMFLOAT3(0, 0, 0);
	data.speed = 0.01f;
	data.speedRnd = 0.0;
	data.size = XMFLOAT2(1.5, 1.5);
	data.sizeRnd = XMFLOAT2(0.4, 0.4);
	data.scale = XMFLOAT2(1.01, 1.01);
	data.color = XMFLOAT4(1, 1, 0, 1);
	data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.02);
	//VFX::Start(data);

}

void HitVFX::CreatePosition(float x, float y, float z)
{
	//�ʒu��ݒ�
	data.position = { x,y,z };

	//VFX�𐶐�
	VFX::Start(data);

}
