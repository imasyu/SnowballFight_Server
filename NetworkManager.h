#pragma once
#include <string>

namespace NetworkManager {
	enum SOCKET_MODE {
		UDP_SERVER,
		UDP_CLIENT,
	};

	//‰Šú‰»ˆ—
	int Initialize();
	int CreateSocket(SOCKET_MODE mode, std::string port);
	int Update();

}
