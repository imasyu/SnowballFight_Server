#include "NetworkManager.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

namespace NetworkManager {

	 // �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;

	int ret; 
	SOCKET sock;

	int Initialize() {
		// WinSock������
		WSADATA wsaData;
		int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret != 0)
		{
			return -1;
		}

		// ���X���\�P�b�g�̍쐬
		sock = socket(AF_INET, SOCK_STREAM, 0);	// 0�Ŏ����ݒ�
		// ���X���\�P�b�g�쐬���s
		if (sock < 0)
		{
			// �G���[�R�[�h���o��
			// �I��
			return 1;
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
			// �G���[�R�[�h���o��
			// �I��
			return 1;
		}

		// ���X����Ԃɐݒ�	�L���[�̃T�C�Y:1
		if (listen(sock, 1) != 0)
		{
			// �G���[�R�[�h���o��
			// �I��
			return 1;
		}

		// �ʐM�p�\�P�b�g ( �N���C�A���g�̃\�P�b�g�Ƃ��̃\�P�b�g�ԂɃR�l�N�V�������m�� )
		int sock;


		struct sockaddr_in clientAddr;		// �ڑ��v�������Ă����N���C�A���g�̃\�P�b�g�A�h���X���i�[�̈�
		int addrlen = sizeof(clientAddr);	// clientAddr�̃T�C�Y

		// �N���C�A���g�����connect()���󂯂āA�R�l�N�V�����m���ς݂̃\�P�b�g�쐬
		sock = accept(sock, (struct sockaddr*)&clientAddr, &addrlen);
		if (sock < 0)
		{
			// �G���[�R�[�h���o��
			// �I��
			return 1;
		}

	}

	int CreateSocket()
	{
		// ���X���\�P�b�g�̍쐬
		//socket = socket(AF_INET, SOCK_STREAM, 0);	// 0�Ŏ����ݒ�
		// ���X���\�P�b�g�쐬���s
		if (socket < 0)
		{
			// �G���[�R�[�h���o��
			// �I��
			return 1;
		}
	}

	int Update()
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

	}


}