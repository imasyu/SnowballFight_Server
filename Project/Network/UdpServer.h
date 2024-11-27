#pragma once
#include <string>
#include "Udp.h"

class UdpServer : public Udp
{
	SOCKADDR_IN fromAddr;	// ���M���\�P�b�g�A�h���X�����i�[����̈�

public:
	bool CreateSocket(std::string port) override;
	bool Update() override;

	int Recv(SOCKET sock, DATA* value) override;
	int Send(SOCKET sock, DATA value) override;

};
