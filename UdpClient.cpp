#include "UdpClient.h"
#include <WS2tcpip.h>

namespace {
	// �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 16;
}

int UdpClient::CreateSocket(std::string port)
{
	// �\�P�b�g�̍쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 1;
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
		return 1;
	}

	// �\�P�b�gsock���m���u���b�L���O�\�P�b�g�ɂ���
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		// �G���[����
	}

}

int UdpClient::Update()
{
	char buff[MESSAGELENGTH];	// ����M���b�Z�[�W�̊i�[�̈�
	buff[0] = 'b';

	// ���M
	ret = send(sock, buff, (int)strlen(buff), 0);
	if (ret != strlen(buff))
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return -1;
	}

	// �N���C�A���g����̃��b�Z�[�W��M
	ret = recv(sock, buff, (int)sizeof(buff) - 1, 0);

	// �m���u���b�L���O�̏ꍇ�A��M�f�[�^���Ȃ��ꍇ���G���[����
	if (ret < 0)
	{
		// �G���[�R�[�h�� wsaewouldblock �̏ꍇ�́A��M�f�[�^���Ȃ��������Ă���
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			OutputDebugString("no Data\n");
			return 1;
		}
		else
		{
			OutputDebugString(WSAGetLastError() + " : Error\n");
			return 1;
		}
	}
	else
	{
		// �I�[�L���̒ǉ�
		buff[ret] = '\0';
		OutputDebugString(buff);
		OutputDebugString("\n");
		return 1;
	}
}
