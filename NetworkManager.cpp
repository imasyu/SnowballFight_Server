#include "NetworkManager.h"
#include <WS2tcpip.h>
#include "UdpServer.h"
#include "UdpClient.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

namespace NetworkManager {

	 // ポート番号
	const unsigned short SERVERPORT = 8888;
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
		char buff[NetworkManager::MESSAGELENGTH];	// 送受信メッセージの格納領域
		// クライアントからのメッセージ受信
		ret_ = recv(sock_, buff, sizeof(buff) - 1, 0);
		if (ret_ < 0)
		{
			// ぬける
			return -1;
		}

		buff[0] = 'a';

		// 終端記号の追加
		buff[ret_] = '\0';

		// 出力
		OutputDebugString(buff);
		OutputDebugString("\n");

		// 送信
		ret_ = send(sock_, buff, strlen(buff), 0);
		if (ret_ != strlen(buff))
		{
			// ぬける
			return -1;
		}

		return 1;
	}


}