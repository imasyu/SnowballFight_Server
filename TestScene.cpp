#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "NetworkManager.h"
#include "Engine/Text.h"

namespace {
	const unsigned short SERVERPORT = 8888;
	
	Text* pText = nullptr;
	int timeCount = 0;

}

//�R���X�g���N�^
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//������
void TestScene::Initialize()
{
	NetworkManager::Initialize();
	NetworkManager::CreateSocket(NetworkManager::SOCKET_MODE::UDP_SERVER, SERVERPORT);

	pText->Initialize();

}

//�X�V
void TestScene::Update()
{

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
