#include "Udp.h"

namespace {
	// ポート番号
	const unsigned short SERVERPORT = 10000;

	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;

}

bool Udp::Send(SOCKET sock, DATA value)
{
	//送信データ:ネットワークバイトオーダーに変換後の値を格納
	DATA sendValue;

	//バイトオーダー変換
	sendValue.posX = htonl(value.posX);
	sendValue.posZ = htonl(value.posZ);
	sendValue.rotateY = htonl(value.rotateY);

	SOCKADDR_IN toAddr;	// 宛先のソケットアドレス情報
	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family = AF_INET;				// IPv4アドレス
	toAddr.sin_port = htons(SERVERPORT);		// サーバのポート番号
	inet_pton(AF_INET, port_.c_str(), &toAddr.sin_addr.s_addr);		// サーバのIPアドレス
	int ret = sendto(sock, (char*)&sendValue, sizeof(sendValue), 0, (SOCKADDR*)&toAddr, sizeof(toAddr));
	if (ret != sizeof(sendValue))
	{
		OutputDebugString("送れなかった\n");
		return false;
	}

	return true;
}

bool Udp::Recv(SOCKET sock, DATA* value)
{
	DATA recvValue;	// 受信データの格納領域...ネットワークバイトオーダー状態

	SOCKADDR_IN fromAddr;	// 送信元ソケットアドレス情報を格納する領域
	int fromlen = sizeof(fromAddr);
	int ret = recvfrom(sock, (char*)&recvValue, sizeof(recvValue), 0, (SOCKADDR*)&fromAddr, &fromlen);
	if (ret != sizeof(recvValue))
	{
		// 正常に送られたけどデータはない
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			OutputDebugString("no Data\n");
			return true;
		}
		// エラー
		else {
			OutputDebugString("Update Error\n");
			return false;
		}
	}

	// 成功時の処理
	value->posX = ntohl(recvValue.posX);
	value->posZ = ntohl(recvValue.posZ);
	value->rotateY = ntohl(recvValue.rotateY);
	
	return 1;
}