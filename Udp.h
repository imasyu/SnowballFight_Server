#pragma once
#include <string>
#include <WS2tcpip.h>

const int MAGNFICATION = 10000;

class Udp
{
protected:
	struct DATA {
		u_long posX;	//”{—¦‚P–œ
		u_long posZ;	//”{—¦‚P–œ
	};

	int ret;
	SOCKET sock;
	DATA data;

public:
	virtual int CreateSocket(std::string port) = 0;
	virtual int Update() = 0;

	bool Recv(int sock, DATA* value);
	bool Send(int sock, DATA value);

};