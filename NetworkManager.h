#pragma once
#include <string>

class Player;

namespace NetworkManager {
	enum SOCKET_MODE {
		UDP_SERVER,
		UDP_CLIENT,
	};

	//初期化処理
	int Initialize(Player* self, Player* other);
	int CreateSocket(SOCKET_MODE mode, std::string port);
	int Update();

	Player* GetSelfPlayer();
	Player* GetOtherPlayer();

}
