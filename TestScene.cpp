#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "NetworkManager.h"
#include "Engine/Text.h"
#include <string>
#include "Player.h"
#include "Stage.h"


namespace {
	const std::string SERVERPORT = "192.168.43.66";	//クライアントの時接続するサーバの数値を入れる

#if 1
	NetworkManager::SOCKET_MODE mode = NetworkManager::SOCKET_MODE::UDP_SERVER;
#else
	NetworkManager::SOCKET_MODE mode = NetworkManager::SOCKET_MODE::UDP_CLIENT;
#endif

	Text* pText = nullptr;
	int timeCount = 0;
}

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene"), pPlayerSelf_(nullptr), pPlayerOther_(nullptr)
{
}

//初期化
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

	if(NetworkManager::Initialize(pPlayerSelf_, pPlayerOther_) == 0) PostQuitMessage(0);
	if(NetworkManager::CreateSocket(mode, SERVERPORT) == 0) PostQuitMessage(0);

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
	timeCount++;

}

//開放
void TestScene::Release()
{
}
