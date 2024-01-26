#pragma once
#include <WinSock2.h>
#include "Udp.h"

class UdpClient : public Udp
{
	int ret;
	SOCKET sock;

public:
	int CreateSocket() override;
	int Update() override;

};

