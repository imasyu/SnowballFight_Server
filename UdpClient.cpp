#include "UdpClient.h"
#include <WS2tcpip.h>

namespace {
	// ポート番号
	const unsigned short SERVERPORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;

}

int UdpClient::CreateSocket(std::string port)
{
	// ソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 0;
	}

	// 接続先サーバのソケットアドレス情報設定
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, port.c_str(), &serverAddr.sin_addr.s_addr);

	// 接続要求
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 0;
	}

	// ソケットsockをノンブロッキングソケットにする
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 0;
	}

	return 1;
}

int UdpClient::Update()
{
	DATA data;
	data.posX = 10.0f;
	data.posZ = -5.0f;

	// 送信
	if (!Send(sock, data)) {
		return 0;
	}
	
	//受信
	if (!Recv(sock, &data)) {
		return 0;
	}

	// 出力
	OutputDebugString("X = ");
	OutputDebugStringA(std::to_string(data.posX).c_str());
	OutputDebugString(" : Y = ");
	OutputDebugStringA(std::to_string(data.posZ).c_str());
	OutputDebugString("\n");

	return 1;
}
