#include "NetworkManager.h"
#include <WS2tcpip.h>
#include "UdpServer.h"
#include "UdpClient.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

namespace NetworkManager {

	 // �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;

	int ret_; 
	SOCKET sock_;
	Udp* socket_;

	int Initialize() {
		// WinSock������
		WSADATA wsaData;
		ret_ = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret_ != 0)
		{
			OutputDebugString("Winsock���������s\n");
			return -1;
		}

		return 1;
	}

	int CreateSocket(SOCKET_MODE mode, unsigned short port)
	{
		if (mode == SOCKET_MODE::UDP_SERVER) {
			socket_ = new UdpServer;
			socket_->CreateSocket();
		}
		if (mode == SOCKET_MODE::UDP_CLIENT) {
			socket_ = new UdpClient;
			socket_->CreateSocket();
		}
		
		return 1;
	}

	int Update()
	{
		char buff[NetworkManager::MESSAGELENGTH];	// ����M���b�Z�[�W�̊i�[�̈�
		// �N���C�A���g����̃��b�Z�[�W��M
		ret_ = recv(sock_, buff, sizeof(buff) - 1, 0);
		if (ret_ < 0)
		{
			// �ʂ���
			return -1;
		}

		buff[0] = 'a';

		// �I�[�L���̒ǉ�
		buff[ret_] = '\0';

		// �o��
		OutputDebugString(buff);
		OutputDebugString("\n");

		// ���M
		ret_ = send(sock_, buff, strlen(buff), 0);
		if (ret_ != strlen(buff))
		{
			// �ʂ���
			return -1;
		}

		return 1;
	}


}