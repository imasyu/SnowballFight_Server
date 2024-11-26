#include "UdpClient.h"
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

bool UdpClient::CreateSocket(std::string port)
{
	port_ = port;

	// �\�P�b�g�̍쐬
	sock_ = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock_ < 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		closesocket(sock_);
		return false;
	}

	// �\�P�b�gsock���m���u���b�L���O�\�P�b�g�ɂ���
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock_, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		closesocket(sock_);
		return false;
	}

	return true;
}

bool UdpClient::Update()
{
	DATA data;
	XMFLOAT3 pos = NetworkManager::GetSelfPlayer()->GetPosition();
	data.posX = (pos.x * MAGNFICATION);
	data.posZ = (pos.z * MAGNFICATION);
	data.rotateY = (NetworkManager::GetSelfPlayer()->GetRotate().y * MAGNFICATION);
	data.shot = Input::IsKeyDown(DIK_SPACE);

	// ���M
	if (!Send(sock_, data)) {
		OutputDebugString("���M�G���[\n");
		return false;
	}

	DATA rData;

	//��M
	if (!Recv(sock_, &rData)) {
		OutputDebugString("��M�G���[\n");
		return false;
	}

	if (rData.shot == 0 || rData.shot == 1) {
		pos = { (float)rData.posX / (float)MAGNFICATION, 0.0f, (float)rData.posZ / (float)MAGNFICATION };
		float rotate = (float)rData.rotateY / (float)MAGNFICATION;
		NetworkManager::GetOtherPlayer()->NotPlayerSetPosition(XMFLOAT3(pos.x, 0.0f, pos.z));
		NetworkManager::GetOtherPlayer()->SetRotateY(rotate);
		if (rData.shot) NetworkManager::GetOtherPlayer()->Shot();
	}
	
	return true;
}

bool UdpClient::Recv(SOCKET sock, DATA* value)
{
	DATA recvValue;	// ��M�f�[�^�̊i�[�̈�...�l�b�g���[�N�o�C�g�I�[�_�[���

	SOCKADDR_IN fromAddr;	// ���M���\�P�b�g�A�h���X�����i�[����̈�
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
	value->shot = ntohs(recvValue.shot);

	return true;
}

bool UdpClient::Send(SOCKET sock, DATA value)
{
	//���M�f�[�^:�l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ���̒l���i�[
	DATA sendValue;

	//�o�C�g�I�[�_�[�ϊ�
	sendValue.posX = htonl(value.posX);
	sendValue.posZ = htonl(value.posZ);
	sendValue.rotateY = htonl(value.rotateY);
	sendValue.shot = htons(value.shot);

	SOCKADDR_IN toAddr;	// ����̃\�P�b�g�A�h���X���
	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family = AF_INET;				// IPv4�A�h���X
	toAddr.sin_port = htons(SERVERPORT);		// �T�[�o�̃|�[�g�ԍ�
	inet_pton(AF_INET, port_.c_str(), &toAddr.sin_addr.s_addr);		// �T�[�o��IP�A�h���X
	int ret = sendto(sock, (char*)&sendValue, sizeof(sendValue), 0, (SOCKADDR*)&toAddr, sizeof(toAddr));
	if (ret != sizeof(sendValue))
	{
		OutputDebugString("����Ȃ�����\n");
		return false;
	}

	return true;
}