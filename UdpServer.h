#pragma once
#include <string>
#include "Udp.h"

class UdpServer : public Udp
{
public:
	int CreateSocket(std::string port) override;
	int Update() override;

};

