#pragma once
#include <memory>
#include <string>

namespace NetworkManager {

	//作成するソケットの種類の指定に使用
	enum class SOCKET_MODE : unsigned char {
		UDP_SERVER,	//アクセスしてきた相手を送信先に自動設定され、複数人に送信を行う
		UDP_CLIENT,	//任意に設定した相手一人のみに送信を行う
	};

	//初期化処理
	int Initialize();

	int CreateSocket(SOCKET_MODE mode, unsigned short port);

	int Update();

}
