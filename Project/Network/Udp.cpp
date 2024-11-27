#include "Udp.h"

bool Udp::Exit()
{
	if (shutdown(sock_, 0x02) != 0)
		return false;

	if (closesocket(sock_) != 0)
		return false;

	return (WSACleanup() != SOCKET_ERROR);
}