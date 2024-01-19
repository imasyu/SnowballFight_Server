#pragma once
#include <winsock2.h>

class UdpServer
{
	int ret;
	SOCKET sock;

public:
	int CreateSocket();
	
	int Update();

};

