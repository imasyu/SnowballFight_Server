#include "UdpServer.h"
#include <WS2tcpip.h>

namespace {
	// ポート番号
	const unsigned short SERVERPORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;

	struct DATA {
		double posX;
		double posZ;
	};

}

bool Recv(int sock, DATA* value)
{
	DATA recvValue;	// 受信データの格納領域...ネットワークバイトオーダー状態
	int ret;		// 成否の判定用
	
	// 受信
	ret = recv(sock, (char*)&recvValue, sizeof(recvValue), 0);
	// 失敗
	if (ret != sizeof(recvValue))
	{
		//正常に送られたけどデータはない
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			OutputDebugString("no Data\n");
			return true;
		}
		//エラー
		else {
			OutputDebugString(WSAGetLastError() + " : Error\n");
			return false;
		}
	}

	// 成功時の処理
	value->posX = ntohl(recvValue.posX);								// int バイトオーダー変換
	value->posZ = ntohl(recvValue.posZ);								// int バイトオーダー変換
	return true;
}

bool Send(int sock, DATA value)
{
	DATA sendValue;	// 送信データ ... ネットワークバイトオーダーに変換後の値を格納
	sendValue.posX = htonl(value.posX);									// int バイトオーダー変換
	sendValue.posZ = htonl(value.posZ);									// int バイトオーダー変換

	int ret;		// 成否の判定用
	// 送信
	ret = send(sock, (char*)&sendValue, sizeof(sendValue), 0);
	// 失敗
	if (ret != sizeof(sendValue))
	{
		return false;
	}

	// 成功
	return true;
}

int UdpServer::CreateSocket(std::string port)
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
		return -1;
	}

	// リスン状態に設定	キューのサイズ:1
	if (listen(sock, 1) != 0)
	{
		OutputDebugString("リスン状態にするの失敗\n");
		return -1;
	}

	struct sockaddr_in clientAddr;		// 接続要求をしてきたクライアントのソケットアドレス情報格納領域
	int addrlen = sizeof(clientAddr);	// clientAddrのサイズ

	// クライアントからのconnect()を受けて、コネクション確立済みのソケット作成
	sock = accept(sock, (struct sockaddr*)&clientAddr, &addrlen);
	if (sock < 0)
	{
		OutputDebugString("コネクション確立失敗\n");
		return -1;
	}

	// ソケットsockをノンブロッキングソケットにする
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		OutputDebugString("ブロッキングソケット化失敗\n");
		return -1;
	}

	return 1;
}

int UdpServer::Update()
{
	//受信
	DATA data;
	Recv(sock, &data);

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
	if (!Send(sock, data)) {
		return -1;
	}

	return 1;
}
