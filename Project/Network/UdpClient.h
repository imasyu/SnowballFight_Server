#pragma once
#include <string>
#include "Udp.h"

class UdpClient : public Udp
{
public:
	bool CreateSocket(std::string port) override;
	bool Update() override;

	int Recv(SOCKET sock, DATA* value) override;
	int Send(SOCKET sock, DATA value) override;

};

