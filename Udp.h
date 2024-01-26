#pragma once
#include <string>
#include <WS2tcpip.h>

class Udp
{
protected:
	struct DATA {
		double posX;
		double posZ;
	};

	int ret;
	SOCKET sock;

public:
	virtual int CreateSocket(std::string port) = 0;
	virtual int Update() = 0;

	bool Recv(int sock, DATA* value);
	bool Send(int sock, DATA value);

};

