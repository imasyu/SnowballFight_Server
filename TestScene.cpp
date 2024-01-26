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

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{
	NetworkManager::Initialize();
	NetworkManager::CreateSocket(NetworkManager::SOCKET_MODE::UDP_SERVER, SERVERPORT);

	pText = new Text();
	pText->Initialize();

}

//更新
void TestScene::Update()
{
	NetworkManager::Update();

}

//描画
void TestScene::Draw()
{
	pText->Draw(30, 30, (timeCount / 60));

}

//開放
void TestScene::Release()
{
}
