#pragma once
#include <string>
#include "Udp.h"

class UdpServer : public Udp
{
	int ret;
	SOCKET sock;

public:
	int CreateSocket(std::string port);
	int Update();

};

