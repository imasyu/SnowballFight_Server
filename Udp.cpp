#include "Udp.h"

bool Udp::Recv(int sock, DATA* value)
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

bool Udp::Send(int sock, DATA value)
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
