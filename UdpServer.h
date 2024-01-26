#pragma once
#include <string>
#include "Udp.h"

class UdpServer : public Udp
{
	int ret;
	int sock;

public:
	int CreateSocket(std::string port);
	int Update();

};

