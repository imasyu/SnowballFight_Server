#pragma once
#include <string>
#include "Udp.h"

class UdpServer : public Udp
{
	SOCKADDR_IN fromAddr;	// 送信元ソケットアドレス情報を格納する領域

public:
	bool CreateSocket(std::string port) override;
	bool Update() override;

	int Recv(SOCKET sock, DATA* value) override;
	int Send(SOCKET sock, DATA value) override;

};

