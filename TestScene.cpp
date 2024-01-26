#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "NetworkManager.h"
#include "Engine/Text.h"
#include <string>
#include "Player.h"
#include "Stage.h"

namespace {
	const std::string SERVERPORT = "192.168.43.50";	//�N���C�A���g�̎��ڑ�����T�[�o�̐��l������
	NetworkManager::SOCKET_MODE mode = NetworkManager::SOCKET_MODE::UDP_SERVER;	//�����ŃT�[�o���N���C�A���g�����߂�

	Text* pText = nullptr;
	int timeCount = 0;
}

//�����l�𑗂��Ă݂Đ��퉻����
//����Ő���Ȃ炻����P���{�Ƃ����Ă��̌����g�p����

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene"), pPlayerSelf_(nullptr), pPlayerOther_(nullptr)
{
}

//������
void TestScene::Initialize()
{
	Instantiate<Stage>(this);
	Player* p1 = Instantiate<Player>(this);
	Player* p2 = Instantiate<Player>(this);
	
	if (mode == 0) {
		p1->InitializeIsPlayer();
		pPlayerSelf_ = p1;
		pPlayerOther_ = p2;
	}
	else {
		p2->InitializeIsPlayer();
		pPlayerSelf_ = p2;
		pPlayerOther_ = p1;
	}

	NetworkManager::Initialize(pPlayerSelf_, pPlayerOther_);
	//NetworkManager::CreateSocket(mode, SERVERPORT);

	pText = new Text();
	pText->Initialize();

}

//�X�V
void TestScene::Update()
{
	//NetworkManager::Update();

}

//�`��
void TestScene::Draw()
{
	pText->Draw(30, 30, (timeCount / 60));
	timeCount++;

}

//�J��
void TestScene::Release()
{
}
