#include "TestScene.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Input.h"
#include "../Network/NetworkManager.h"
#include "../Player/Player.h"
#include "Stage.h"
#include "VFXManager.h"

#include <fstream>
#include <string>
#include <sstream>
using namespace std;

namespace {
	std::string SERVERPORT;	//�N���C�A���g�̎��ڑ�����T�[�o�̐��l������
	NetworkManager::SOCKET_MODE mode = NetworkManager::SOCKET_MODE::UDP_SERVER;

}

void LoadPort()
{
	//�t�@�C���ǂݍ���
	std::ifstream ifs2("GameSeting/Port");
	std::string data2;
	ifs2 >> data2;
	//string����int�֕ϊ����A���̂��ƒl���Z�b�g
	std::istringstream ss2 = std::istringstream(data2);
	ss2 >> SERVERPORT;	//�N���C�A���g�̎��ڑ�����T�[�o�̐��l������;
}

void LoadMode()
{
	//�t�@�C���ǂݍ���
	std::ifstream ifs2("GameSeting/Mode");
	std::string data2;
	ifs2 >> data2;
	//string����int�֕ϊ����A���̂��ƒl���Z�b�g
	std::istringstream ss2 = std::istringstream(data2);
	int nm = 0;
	ss2 >> nm;	//�N���C�A���g�̎��ڑ�����T�[�o�̐��l������;
	if (nm == 1) mode = NetworkManager::SOCKET_MODE::UDP_CLIENT;

}

//�R���X�g���N�^
TestScene::TestScene(GameObject* parent)
	: GameObject(parent, "TestScene"), pPlayerSelf_(nullptr), pPlayerOther_(nullptr)
{
}

//������
void TestScene::Initialize()
{
	LoadPort();
	LoadMode();
	VFXManager::Initialize();

	Instantiate<Stage>(this);
	Player* p1 = Instantiate<Player>(this);
	Player* p2 = Instantiate<Player>(this);

	if (mode == 0) {
		p1->InitializeIsPlayer();
		OutputDebugString("1");

		pPlayerSelf_ = p1;
		pPlayerOther_ = p2;
	}
	else {
		OutputDebugString("2");

		p2->InitializeIsPlayer();
		pPlayerSelf_ = p2;
		pPlayerOther_ = p1;
	}

	if (NetworkManager::Initialize(pPlayerSelf_, pPlayerOther_) == 0) PostQuitMessage(0);
	if (NetworkManager::CreateSocket(mode, SERVERPORT) == 0) PostQuitMessage(0);
}


//�X�V
void TestScene::Update()
{
	NetworkManager::Update();

}

//�`��
void TestScene::Draw()
{
}

//�J��
void TestScene::Release()
{
	NetworkManager::Exit();

}
