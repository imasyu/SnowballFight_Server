#include "NetworkManager.h"
#include <WS2tcpip.h>
#include "UdpServer.h"
#include "UdpClient.h"
#include "Player.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

namespace NetworkManager {

	Udp* socket_;
	Player* self_;
	Player* other_;

	bool Initialize(class Player* self, class Player* other) {
		self_ = self;
		other_ = other;

		// WinSock‰Šú‰»
		WSADATA wsaData;
		int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret != 0)
		{
			OutputDebugString("Winsock‰Šú‰»Ž¸”s\n");
			return false;
		}

		return true;
	}

	bool CreateSocket(SOCKET_MODE mode, std::string port)
	{
		if (mode == SOCKET_MODE::UDP_SERVER) {
			socket_ = new UdpServer;
			return socket_->CreateSocket(port);
		}
		if (mode == SOCKET_MODE::UDP_CLIENT) {
			socket_ = new UdpClient;
			return socket_->CreateSocket(port);
		}
		
		return true;
	}

	bool Update()
	{
		if(socket_) return socket_->Update();
		return false;
	}

	bool Exit()
	{
		return socket_->Exit();
	}

	Player* GetSelfPlayer() { return self_; }
	Player* GetOtherPlayer() { return other_; }

}