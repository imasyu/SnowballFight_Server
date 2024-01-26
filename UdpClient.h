#pragma once
#include <string>
#include "Udp.h"

class UdpClient : public Udp
{
public:
	int CreateSocket(std::string port) override;
	int Update() override;

};

