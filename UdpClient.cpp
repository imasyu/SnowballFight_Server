#include "UdpClient.h"
#include <WS2tcpip.h>

namespace {
	// �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;

}

int UdpClient::CreateSocket(std::string port)
{
	// �\�P�b�g�̍쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 0;
	}

	// �ڑ���T�[�o�̃\�P�b�g�A�h���X���ݒ�
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, port.c_str(), &serverAddr.sin_addr.s_addr);

	// �ڑ��v��
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 0;
	}

	// �\�P�b�gsock���m���u���b�L���O�\�P�b�g�ɂ���
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 0;
	}

	return 1;
}

int UdpClient::Update()
{
	DATA data;
	data.posX = 10.0f;
	data.posZ = -5.0f;

	// ���M
	if (!Send(sock, data)) {
		return 0;
	}
	
	//��M
	if (!Recv(sock, &data)) {
		return 0;
	}

	// �o��
	OutputDebugString("X = ");
	OutputDebugStringA(std::to_string(data.posX).c_str());
	OutputDebugString(" : Y = ");
	OutputDebugStringA(std::to_string(data.posZ).c_str());
	OutputDebugString("\n");

	return 1;
}
