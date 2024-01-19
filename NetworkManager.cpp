#include "NetworkManager.h"
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

namespace NetworkManager {
	 // �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;

	int ret; 
	SOCKET socket;

	int Initialize() {
		// WinSock������
		WSADATA wsaData;
		ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (ret != 0)
		{
			return 1;
		}

		CreateSocket();
		BindAndListen(port);

		// �V�����N���C�A���g���ڑ������܂őҋ@
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