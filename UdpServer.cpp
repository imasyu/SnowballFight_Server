#include "UdpServer.h"

namespace {
	// ポート番号
	const unsigned short SERVERPORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;
}

int UdpServer::CreateSocket()
{
	// リスンソケットの作成
	sock = socket(AF_INET, SOCK_STREAM, 0);	// 0で自動設定
	if (sock <= 0)
	{
		OutputDebugString("リスンソケット作成失敗\n");
		return -1;
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
		return 1;
	}

	// リスン状態に設定	キューのサイズ:1
	if (listen(sock, 1) != 0)
	{
		OutputDebugString("リスン状態にするの失敗\n");
		return 1;
	}

	struct sockaddr_in clientAddr;		// 接続要求をしてきたクライアントのソケットアドレス情報格納領域
	int addrlen = sizeof(clientAddr);	// clientAddrのサイズ

	// クライアントからのconnect()を受けて、コネクション確立済みのソケット作成
	sock = accept(sock, (struct sockaddr*)&clientAddr, &addrlen);
	if (sock < 0)
	{
		OutputDebugString("コネクション確立失敗n");
		return 1;
	}

	return 1;
}

int UdpServer::Update()
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
