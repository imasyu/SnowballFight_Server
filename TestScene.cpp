#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "NetworkManager.h"
#include "Engine/Text.h"
#include <string>
#include "Player.h"

namespace {
	const std::string SERVERPORT = "192.168.43.50";

	Text* pText = nullptr;
	int timeCount = 0;

	Player* pPlayer1 = nullptr;
	Player* pPlayer2 = nullptr;

}

//�����l�𑗂��Ă݂Đ��퉻����
//����Ő���Ȃ炻����P�O���{���Ă��̌����g�p����

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������
void TestScene::Initialize()
{
	NetworkManager::SOCKET_MODE mode = NetworkManager::SOCKET_MODE::UDP_SERVER;
	
	//NetworkManager::Initialize();
	//NetworkManager::CreateSocket(mode, SERVERPORT);
	
	pPlayer1 = Instantiate<Player>(this);
	pPlayer2 = Instantiate<Player>(this);
	
	if(mode == 0) pPlayer1->InitializeIsPlayer();
	else  pPlayer2->InitializeIsPlayer();

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

}

//�J��
void TestScene::Release()
{
}
