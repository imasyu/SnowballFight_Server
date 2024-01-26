#pragma once
#include <winsock2.h>
#include "Udp.h"

class UdpServer : public Udp
{
	int ret;
	SOCKET sock;

public:
	int CreateSocket();
	int Update();

};

