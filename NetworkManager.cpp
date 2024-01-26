#include "NetworkManager.h"
#include <WS2tcpip.h>
#include "UdpServer.h"
#include "UdpClient.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

namespace NetworkManager {
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

	int CreateSocket(SOCKET_MODE mode, std::string port)
	{
		if (mode == SOCKET_MODE::UDP_SERVER) {
			socket_ = new UdpServer;
			socket_->CreateSocket(port);
		}
		if (mode == SOCKET_MODE::UDP_CLIENT) {
			socket_ = new UdpClient;
			socket_->CreateSocket(port);
		}
		
		return 1;
	}

	int Update()
	{
		socket_->Update();

		return 1;
	}


}