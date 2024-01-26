#include "UdpServer.h"

namespace {
	// �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;
}

int UdpServer::CreateSocket()
{
	// ���X���\�P�b�g�̍쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);	// 0�Ŏ����ݒ�
	if (sock <= 0)
	{
		OutputDebugString("���X���\�P�b�g�쐬���s\n");
		return -1;
	}

	// bind
	struct sockaddr_in bindAddr;	// bind�p�̃\�P�b�g�A�h���X���
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// �\�P�b�g�A�h���X���ݒ�	���Œ�̃|�[�g�ԍ��ݒ�
	if (bind(sock, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
	{
		OutputDebugString("�\�P�b�g�A�h���X�̐ݒ�\n");
		return 1;
	}

	// ���X����Ԃɐݒ�	�L���[�̃T�C�Y:1
	if (listen(sock, 1) != 0)
	{
		OutputDebugString("���X����Ԃɂ���̎��s\n");
		return 1;
	}

	struct sockaddr_in clientAddr;		// �ڑ��v�������Ă����N���C�A���g�̃\�P�b�g�A�h���X���i�[�̈�
	int addrlen = sizeof(clientAddr);	// clientAddr�̃T�C�Y

	// �N���C�A���g�����connect()���󂯂āA�R�l�N�V�����m���ς݂̃\�P�b�g�쐬
	sock = accept(sock, (struct sockaddr*)&clientAddr, &addrlen);
	if (sock < 0)
	{
		OutputDebugString("�R�l�N�V�����m�����sn");
		return 1;
	}

	return 1;
}

int UdpServer::Update()
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
