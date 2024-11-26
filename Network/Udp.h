#pragma once
#include <string>
#include <WS2tcpip.h>

static const float MAGNFICATION = 1000.f;

class Udp
{
protected:
	struct DATA {
		u_long posX;	//”{—¦‚P–œ
		u_long posZ;	//”{—¦‚P–œ
		u_long rotateY;	//
		u_short shot;	//‘Å‚Á‚½‚©‚Ìƒtƒ‰ƒO
	};

	int ret_;
	SOCKET sock_;
	DATA data_;
	std::string port_;

public:
	virtual bool CreateSocket(std::string port) = 0;
	virtual bool Update() = 0;
	bool Exit();

	virtual bool Recv(SOCKET sock, DATA* value) = 0;
	virtual bool Send(SOCKET sock, DATA value) = 0;

};