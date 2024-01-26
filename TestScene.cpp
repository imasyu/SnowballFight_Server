#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "NetworkManager.h"
#include "Engine/Text.h"
#include <string>

namespace {
	const std::string SERVERPORT = "192.168.43.50";

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

	pText = new Text();
	pText->Initialize();

}

//�X�V
void TestScene::Update()
{
	NetworkManager::Update();

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
