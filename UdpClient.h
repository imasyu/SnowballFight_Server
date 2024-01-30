#pragma once
#include <string>
#include "Udp.h"

class UdpClient : public Udp
{
public:
	bool CreateSocket(std::string port) override;
	bool Update() override;

	bool Recv(SOCKET sock, DATA* value) override;
	bool Send(SOCKET sock, DATA value) override;

};

