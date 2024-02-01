#include "UdpServer.h"
#include <WS2tcpip.h>
#include "Player.h"
#include "NetworkManager.h"
#include "Engine/Input.h"

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
	if (Recv(sock_, &data)) {
		//��M�ł���
		XMFLOAT3 pos = { (float)data.posX / (float)MAGNFICATION, 0.0f, (float)data.posZ / (float)MAGNFICATION };
		//OutputDebugString(("X = " + std::to_string(pos.x) + " : Y = " + std::to_string(pos.z) + "\n").c_str());
		//OutputDebugString((std::to_string(data.shot) + "\n").c_str());

		if (data.shot == 0 || data.shot == 1) {
			float rotate = (float)data.rotateY / (float)MAGNFICATION;
			NetworkManager::GetOtherPlayer()->NotPlayerSetPosition(XMFLOAT3(pos.x, 0.0f, pos.z));
			NetworkManager::GetOtherPlayer()->SetRotateY(rotate);
			if (data.shot) NetworkManager::GetOtherPlayer()->Shot();
		}

	}
	else {
		OutputDebugString("��M�G���[\n");
		return false;
	}

	//���M
	XMFLOAT3 pos = NetworkManager::GetSelfPlayer()->GetPosition();
	data.posX = (pos.x * MAGNFICATION);
	data.posZ = (pos.z * MAGNFICATION);
	data.rotateY = (NetworkManager::GetSelfPlayer()->GetRotate().y * MAGNFICATION);
	data.shot = Input::IsKeyDown(DIK_SPACE);

	if (!Send(sock_, data)) {
		OutputDebugString("���M�G���[\n");
		return false;
	}

	return true;
}

bool UdpServer::Recv(SOCKET sock, DATA* value)
{
	DATA recvValue;	// ��M�f�[�^�̊i�[�̈�...�l�b�g���[�N�o�C�g�I�[�_�[���

	int fromlen = sizeof(fromAddr);
	int ret = recvfrom(sock, (char*)&recvValue, sizeof(recvValue), 0, (SOCKADDR*)&fromAddr, &fromlen);
	if (ret != sizeof(recvValue))
	{
		// ����ɑ���ꂽ���ǃf�[�^�͂Ȃ�
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			OutputDebugString("no Data\n");
			return true;
		}
		// �G���[
		else {
			OutputDebugString("Update Error\n");
			return false;
		}
	}

	// �������̏���
	value->posX = ntohl(recvValue.posX);
	value->posZ = ntohl(recvValue.posZ);
	value->rotateY = ntohl(recvValue.rotateY);
	value->shot = htons(recvValue.shot);

	return 1;
}

bool UdpServer::Send(SOCKET sock, DATA value)
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
		return false;
	}

	return true;
}