#include "UdpClient.h"
#include <WS2tcpip.h>

namespace {
	// ポート番号
	const unsigned short SERVERPORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;
}

int UdpClient::CreateSocket()
{
	// ソケットの作成
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 1;
	}

	// 接続先サーバのソケットアドレス情報設定
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, "192.168.43.50", &serverAddr.sin_addr.s_addr);	// ほんとはよくない。せめて127.0.0.1を定数化

	// 接続要求
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 1;
	}

}

int UdpClient::Update()
{
	char buff[MESSAGELENGTH];	// 送受信メッセージの格納領域
	// クライアントからのメッセージ受信
	ret = recv(sock, buff, sizeof(buff) - 1, 0);
	if (ret < 0)
	{
		// ぬける
		return -1;
	}

	buff[0] = 'a';

	// 終端記号の追加
	buff[ret] = '\0';

	// 出力
	OutputDebugString(buff);
	OutputDebugString("\n");

	// 送信
	ret = send(sock, buff, strlen(buff), 0);
	if (ret != strlen(buff))
	{
		// ぬける
		return -1;
	}

	return 1;
}
