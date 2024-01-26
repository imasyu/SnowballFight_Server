#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"
#include "NetworkManager.h"
#include "Engine/Text.h"
#include <string>
#include "Player.h"
#include "Stage.h"

namespace {
	const std::string SERVERPORT = "192.168.43.50";	//クライアントの時接続するサーバの数値を入れる
	NetworkManager::SOCKET_MODE mode = NetworkManager::SOCKET_MODE::UDP_SERVER;	//ここでサーバかクライアントを決める

	Text* pText = nullptr;
	int timeCount = 0;
}

//整数値を送ってみて正常化試す
//それで正常ならそれを１万倍とかしてその桁を使用する

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

	NetworkManager::Initialize(pPlayerSelf_, pPlayerOther_);
	//NetworkManager::CreateSocket(mode, SERVERPORT);

	pText = new Text();
	pText->Initialize();

}

//更新
void TestScene::Update()
{
	//NetworkManager::Update();

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
