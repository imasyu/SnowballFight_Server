#include "UdpServer.h"
#include <WS2tcpip.h>
#include "Player.h"
#include "NetworkManager.h"

namespace {
	// �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;

}

int UdpServer::CreateSocket(std::string port)
{
	port_ = port;
	
	// ���X���\�P�b�g�̍쐬
	sock_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_ == INVALID_SOCKET)
	{
		OutputDebugString("���X���\�P�b�g�쐬���s\n");
		return 0;
	}

	// bind
	struct sockaddr_in bindAddr;	// bind�p�̃\�P�b�g�A�h���X���
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// �\�P�b�g�A�h���X���ݒ�	���Œ�̃|�[�g�ԍ��ݒ�
	if (bind(sock_, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
	{
		OutputDebugString("�\�P�b�g�A�h���X�̐ݒ�G���[\n");
		closesocket(sock_);  // �\�P�b�g�����
		return 0;
	}

	// �\�P�b�gsock���m���u���b�L���O�\�P�b�g�ɂ���
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock_, FIONBIO, &cmdarg);
	if (ret == SOCKET_ERROR)
	{
		OutputDebugString("�m���u���b�L���O�\�P�b�g�����s\n");
		closesocket(sock_);  // �\�P�b�g�����
		return 0;
	}

	return 1;
}

int UdpServer::Update()
{
	//��M
	DATA data;
	if (Recv(sock_, &data)) {
		data.posX = (data.posX / MAGNFICATION);
		data.posZ = (data.posZ / MAGNFICATION);
		data.rotateY = (data.rotateY / MAGNFICATION);
		OutputDebugString(("X = " + std::to_string(data.posX) + " : Y = " + std::to_string(data.posZ) + "\n").c_str());
	}
	else {
		OutputDebugString("��M�G���[\n");
		return 0;
	}

	//���M
	XMFLOAT3 pos = NetworkManager::GetSelfPlayer()->GetPosition();
	data.posX = (pos.x * MAGNFICATION);
	data.posZ = (pos.z * MAGNFICATION);
	data.rotateY = (NetworkManager::GetSelfPlayer()->GetRotate().y * MAGNFICATION);

	if (!Send(sock_, data)) {
		OutputDebugString("���M�G���[\n");
		return 0;
	}

	return 1;
}
