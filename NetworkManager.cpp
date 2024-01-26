#include "NetworkManager.h"
#include <WS2tcpip.h>
#include "UdpServer.h"
#include "UdpClient.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

namespace NetworkManager {
	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;

	int ret_; 
	SOCKET sock_;
	Udp* socket_;

	int Initialize() {
		// WinSock初期化
		WSADATA wsaData;
		ret_ = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret_ != 0)
		{
			OutputDebugString("Winsock初期化失敗\n");
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