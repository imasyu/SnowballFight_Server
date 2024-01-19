#pragma once
#include <winsock2.h>

class UdpClient
{
	int ret;
	SOCKET sock;

public:
	int Initialize();
	int CreateSocket();
	int Update();

};

