#include "NetworkManager.h"
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

namespace NetworkManager {
	 // ポート番号
	const unsigned short SERVERPORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;

	int ret; 
	SOCKET socket;

	int Initialize() {
		// WinSock初期化
		WSADATA wsaData;
		ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret != 0)
		{
			return 1;
		}

		CreateSocket();
		BindAndListen(port);

		// 新しいクライアントが接続されるまで待機
		WaitForConnection();

		Cleanup();

	}

	void CreateSocket()
	{
		socket = socket(AF_INET, SOCK_STREAM, 0);
		if (listenSocket == INVALID_SOCKET) {
			std::cerr << "socket failed with error: " << WSAGetLastError() << std::endl;
			WSACleanup();
			exit(EXIT_FAILURE);
		}
	}

	int Finalize() {

		for (auto& socket : sockets) {

			socket->Close();
			socket.reset();
		}

		auto result = WSACleanup();
		if (result != 0)	throw NetworkException("WSACleanup() : ErrorCode", WSAGetLastError());
	}

	void Update() {

		for (const auto& socket : sockets) {

			if (!socket)	continue;

			socket->Update();
		}
	}

}