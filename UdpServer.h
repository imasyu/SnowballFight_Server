#pragma once
#include <string>
#include "Udp.h"

class UdpServer : public Udp
{
	SOCKADDR_IN fromAddr;	// 送信元ソケットアドレス情報を格納する領域

public:
	int CreateSocket(std::string port) override;
	int Update() override;

	bool Recv(SOCKET sock, DATA* value) override;
	bool Send(SOCKET sock, DATA value) override;

};

