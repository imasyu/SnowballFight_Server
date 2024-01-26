#pragma once
#include <string>

class Udp
{
public:
	virtual int CreateSocket(std::string port) = 0;
	virtual int Update() = 0;

};

