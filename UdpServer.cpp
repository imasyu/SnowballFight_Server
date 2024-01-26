#include "UdpServer.h"
#include <WS2tcpip.h>

namespace {
	// ポート番号
	const unsigned short SERVERPORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;

}

int UdpServer::CreateSocket(std::string port)
{
	// リスンソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);	// 0で自動設定
	if (sock <= 0)
	{
		OutputDebugString("リスンソケット作成失敗\n");
		return 0;
	}

	// bind
	struct sockaddr_in bindAddr;	// bind用のソケットアドレス情報
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// ソケットアドレス情報設定	※固定のポート番号設定
	if (bind(sock, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
	{
		OutputDebugString("ソケットアドレスの設定\n");
		return 0;
	}

	// リスン状態に設定	キューのサイズ:1
	if (listen(sock, 1) != 0)
	{
		OutputDebugString("リスン状態にするの失敗\n");
		return 0;
	}

	struct sockaddr_in clientAddr;		// 接続要求をしてきたクライアントのソケットアドレス情報格納領域
	int addrlen = sizeof(clientAddr);	// clientAddrのサイズ

	// クライアントからのconnect()を受けて、コネクション確立済みのソケット作成
	sock = accept(sock, (struct sockaddr*)&clientAddr, &addrlen);
	if (sock < 0)
	{
		OutputDebugString("コネクション確立失敗\n");
		return 0;
	}

	// ソケットsockをノンブロッキングソケットにする
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		OutputDebugString("ブロッキングソケット化失敗\n");
		return 0;
	}

	return 1;
}

int UdpServer::Update()
{
	//受信
	DATA data;
	if(!Recv(sock, &data)) return -1;

	// 出力
	OutputDebugString("X = ");
	OutputDebugStringA(std::to_string(data.posX).c_str());
	OutputDebugString(" : Y = ");
	OutputDebugStringA(std::to_string(data.posZ).c_str());
	OutputDebugString("\n");

	//送信
	data.posX = 12345678.0f;
	data.posZ = 0.0f;

	// 送信
	if (!Send(sock, data)) return -1;

	return 1;
}
