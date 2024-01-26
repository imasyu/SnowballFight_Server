#include "UdpClient.h"
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
	DATA data;
	data.posX = 10.0f;
	data.posZ = -5.0f;

	// 送信
	if (!Send(sock, data)) {
		return -1;
	}
	
	//受信
	if (!Recv(sock, &data)) {
		return -1;
	}

	// 出力
	OutputDebugString("X = ");
	OutputDebugStringA(std::to_string(data.posX).c_str());
	OutputDebugString(" : Y = ");
	OutputDebugStringA(std::to_string(data.posZ).c_str());
	OutputDebugString("\n");


}
