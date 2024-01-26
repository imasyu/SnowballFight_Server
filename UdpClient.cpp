#include "UdpClient.h"
#include <WS2tcpip.h>

namespace {
	// ポート番号
	const unsigned short SERVERPORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 16;
}

int UdpClient::CreateSocket(std::string port)
{
	// ソケットの作成
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
	inet_pton(AF_INET, port.c_str(), &serverAddr.sin_addr.s_addr);

	// 接続要求
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 1;
	}

	// ソケットsockをノンブロッキングソケットにする
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		// エラー処理
	}

}

int UdpClient::Update()
{
	char buff[MESSAGELENGTH];	// 送受信メッセージの格納領域
	buff[0] = 'b';

	// 送信
	ret = send(sock, buff, (int)strlen(buff), 0);
	if (ret != strlen(buff))
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return -1;
	}

	// クライアントからのメッセージ受信
	ret = recv(sock, buff, (int)sizeof(buff) - 1, 0);

	// ノンブロッキングの場合、受信データがない場合もエラー扱い
	if (ret < 0)
	{
		// エラーコードが wsaewouldblock の場合は、受信データがなかったってこと
		if (WSAGetLastError() == WSAEWOULDBLOCK)
		{
			OutputDebugString("no Data\n");
			return 1;
		}
		else
		{
			OutputDebugString(WSAGetLastError() + " : Error\n");
			return 1;
		}
	}
	else
	{
		// 終端記号の追加
		buff[ret] = '\0';
		OutputDebugString(buff);
		OutputDebugString("\n");
		return 1;
	}
}
