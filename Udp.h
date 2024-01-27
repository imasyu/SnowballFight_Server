#pragma once
#include <string>
#include <WS2tcpip.h>

static const u_long MAGNFICATION = 10000;

class Udp
{
protected:
	struct DATA {
		u_long posX;	//�{���P��
		u_long posZ;	//�{���P��
		u_long rotateY;	//
	};

	int ret_;
	SOCKET sock_;
	DATA data_;
	std::string port_;

public:
	virtual int CreateSocket(std::string port) = 0;
	virtual int Update() = 0;
	
	bool Send(SOCKET sock, DATA value);
	bool Recv(SOCKET sock, DATA* value);
};