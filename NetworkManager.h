#pragma once
#include <string>

class Player;

namespace NetworkManager {
	enum SOCKET_MODE {
		UDP_SERVER,
		UDP_CLIENT,
	};

	//初期化処理
	bool Initialize(Player* self, Player* other);
	bool CreateSocket(SOCKET_MODE mode, std::string port);
	bool Update();
	bool Exit();
	
	Player* GetSelfPlayer();
	Player* GetOtherPlayer();

}
