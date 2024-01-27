#include "UdpClient.h"
#include <WS2tcpip.h>
#include "Player.h"
#include "NetworkManager.h"

int UdpClient::CreateSocket(std::string port)
{
	port_ = port;

	// �\�P�b�g�̍쐬
	sock_ = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock_ < 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		closesocket(sock_);
		return 0;
	}

	// �\�P�b�gsock���m���u���b�L���O�\�P�b�g�ɂ���
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock_, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		closesocket(sock_);
		return 0;
	}
	
	return 1;
}

int UdpClient::Update()
{
	DATA data;
	XMFLOAT3 pos = NetworkManager::GetSelfPlayer()->GetPosition();
	data.posX = (pos.x * MAGNFICATION);
	data.posZ = (pos.z * MAGNFICATION);
	data.rotateY = (NetworkManager::GetSelfPlayer()->GetRotate().y * MAGNFICATION);

	// ���M
	if (!Send(sock_, data)) {
		OutputDebugString("���M�G���[\n");
		return 0;
	}
	
	//��M
	if (!Recv(sock_, &data)) {
		OutputDebugString("��M�G���[\n");
		return 0;
	}

	data.posX = data.posX / MAGNFICATION;
	data.posZ = data.posZ / MAGNFICATION;
	data.rotateY = data.rotateY / MAGNFICATION;
	OutputDebugString(("X = " + std::to_string(data.posX) + " : Y = " + std::to_string(data.posZ) + "\n").c_str());

	return 1;
}
