#include "UdpClient.h"
#include <WS2tcpip.h>

namespace {
	// �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;
}

int UdpClient::CreateSocket()
{
	// �\�P�b�g�̍쐬
	int sock;
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
	inet_pton(AF_INET, "192.168.43.50", &serverAddr.sin_addr.s_addr);	// �ق�Ƃ͂悭�Ȃ��B���߂�127.0.0.1��萔��

	// �ڑ��v��
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 1;
	}

}

int UdpClient::Update()
{
	char buff[MESSAGELENGTH];	// ����M���b�Z�[�W�̊i�[�̈�
	// �N���C�A���g����̃��b�Z�[�W��M
	ret = recv(sock, buff, sizeof(buff) - 1, 0);
	if (ret < 0)
	{
		// �ʂ���
		return -1;
	}

	buff[0] = 'a';

	// �I�[�L���̒ǉ�
	buff[ret] = '\0';

	// �o��
	OutputDebugString(buff);
	OutputDebugString("\n");

	// ���M
	ret = send(sock, buff, strlen(buff), 0);
	if (ret != strlen(buff))
	{
		// �ʂ���
		return -1;
	}

	return 1;
}
