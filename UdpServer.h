#pragma once
#include <string>
#include "Udp.h"

class UdpServer : public Udp
{
	SOCKADDR_IN fromAddr;	// ���M���\�P�b�g�A�h���X�����i�[����̈�

public:
	int CreateSocket(std::string port) override;
	int Update() override;

	bool Recv(SOCKET sock, DATA* value) override;
	bool Send(SOCKET sock, DATA value) override;

};

