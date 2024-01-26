#pragma once
#include <string>

namespace NetworkManager {
	enum SOCKET_MODE {
		UDP_SERVER,
		UDP_CLIENT,
	};

	//初期化処理
	int Initialize();
	int CreateSocket(SOCKET_MODE mode, std::string port);
	int Update();

}
