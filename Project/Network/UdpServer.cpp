#include "UdpServer.h"
#include "NetworkManager.h"
#include <WS2tcpip.h>
#include "../Player/Player.h"
#include "../Engine/Input.h"

namespace {
	// �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;

}

bool UdpServer::CreateSocket(std::string port)
{
	port_ = port;

	// ���X���\�P�b�g�̍쐬
	sock_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_ == INVALID_SOCKET)
	{
		OutputDebugString("���X���\�P�b�g�쐬���s\n");
		return false;
	}

	// bind
	struct sockaddr_in bindAddr;	// bind�p�̃\�P�b�g�A�h���X���
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//�\�P�b�g�A�h���X���ݒ�	���Œ�̃|�[�g�ԍ��ݒ�
	ret_ = bind(sock_, (SOCKADDR*)&bindAddr, sizeof(bindAddr));
	if (ret_ == SOCKET_ERROR)
	{
		OutputDebugString("�\�P�b�g�A�h���X�̐ݒ�G���[\n");
		closesocket(sock_);  // �\�P�b�g�����
		return false;
	}

	// �\�P�b�gsock���m���u���b�L���O�\�P�b�g�ɂ���
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock_, FIONBIO, &cmdarg);
	if (ret == SOCKET_ERROR)
	{
		OutputDebugString("�m���u���b�L���O�\�P�b�g�����s\n");
		closesocket(sock_);  // �\�P�b�g�����
		return false;
	}

	return true;
}

bool UdpServer::Update()
{
	//��M
	DATA data;
	int resultRe = Recv(sock_, &data);
	if (resultRe == 1) {
		//��M�ł���
		XMFLOAT3 pos = { (float)data.posX / (float)MAGNFICATION, 0.0f, (float)data.posZ / (float)MAGNFICATION };

		if (data.shot == 0 || data.shot == 1) {
			float rotate = (float)data.rotateY / (float)MAGNFICATION;
			NetworkManager::GetOtherPlayer()->NotPlayerSetPosition(XMFLOAT3(pos.x, 0.0f, pos.z));
			NetworkManager::GetOtherPlayer()->SetRotateY(rotate);
			if (data.shot) NetworkManager::GetOtherPlayer()->Shot();
		}
	}
	else if(resultRe == -1) {
		return false;
	}

	//���M
	XMFLOAT3 pos = NetworkManager::GetSelfPlayer()->GetPosition();
	data.posX = (pos.x * MAGNFICATION);
	data.posZ = (pos.z * MAGNFICATION);
	data.rotateY = (NetworkManager::GetSelfPlayer()->GetRotate().y * MAGNFICATION);
	data.shot = Input::IsKeyDown(DIK_SPACE);

	if (Send(sock_, data) == -1) {
		return false;
	}

	return true;
}

int UdpServer::Recv(SOCKET sock, DATA* value)
{
	DATA recvValue;	// ��M�f�[�^�̊i�[�̈�...�l�b�g���[�N�o�C�g�I�[�_�[���

	int fromlen = sizeof(fromAddr);
	int ret = recvfrom(sock, (char*)&recvValue, sizeof(recvValue), 0, (SOCKADDR*)&fromAddr, &fromlen);
	if (ret != sizeof(recvValue))
	{
		// ����ɑ���ꂽ���ǃf�[�^�͂Ȃ�
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			OutputDebugString("no Data\n");
			return 0;
		}
		// �G���[
		else {
			OutputDebugString("Update Error\n");
			return -1;
		}
	}

	// �������̏���
	value->posX = ntohl(recvValue.posX);
	value->posZ = ntohl(recvValue.posZ);
	value->rotateY = ntohl(recvValue.rotateY);
	value->shot = htons(recvValue.shot);

	return 1;
}

int UdpServer::Send(SOCKET sock, DATA value)
{
	//���M�f�[�^:�l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ���̒l���i�[
	DATA sendValue;

	//�o�C�g�I�[�_�[�ϊ�
	sendValue.posX = htonl(value.posX);
	sendValue.posZ = htonl(value.posZ);
	sendValue.rotateY = htonl(value.rotateY);
	sendValue.shot = htons(value.shot);

	int fromlen = sizeof(fromAddr);
	ret_ = sendto(sock, (char*)&sendValue, sizeof(sendValue), 0, (SOCKADDR*)&fromAddr, fromlen);
	if (ret_ != sizeof(sendValue))
	{
		OutputDebugString("����Ȃ�����\n");
		return 0;
	}

	return 1;
}