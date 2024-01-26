#pragma once
#include <string>
#include "Udp.h"
#include <WS2tcpip.h>

class UdpClient : public Udp
{
	int ret;
	SOCKET sock;

public:
	int CreateSocket(std::string port) override;
	int Update() override;

};

