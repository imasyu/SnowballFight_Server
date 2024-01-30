#include "UdpClient.h"
#include <WS2tcpip.h>
#include "Player.h"
#include "NetworkManager.h"

namespace {
	// ポート番号
	const unsigned short SERVERPORT = 8888;

	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;

}

int UdpClient::CreateSocket(std::string port)
{
	port_ = port;

	// ソケットの作成
	sock_ = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock_ < 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		closesocket(sock_);
		return 0;
	}

	// ソケットsockをノンブロッキングソケットにする
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock_, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		closesocket(sock_);
		return 0;
	}
	
	return 1;
}

int UdpClient::Update()
{
	DATA data;
	XMFLOAT3 pos = NetworkManager::GetSelfPlayer()->GetPosition();
	data.posX = (pos.x * MAGNFICATION);
	data.posZ = (pos.z * MAGNFICATION);
	data.rotateY = (NetworkManager::GetSelfPlayer()->GetRotate().y * MAGNFICATION);

	// 送信
	if (!Send(sock_, data)) {
		OutputDebugString("送信エラー\n");
		return 0;
	}
	
	//受信
	if (!Recv(sock_, &data)) {
		OutputDebugString("受信エラー\n");
		return 0;
	}

	pos = { (float)data.posX / (float)MAGNFICATION, 0.0f, (float)data.posZ / (float)MAGNFICATION };
	OutputDebugString(("X = " + std::to_string(pos.x) + " : Y = " + std::to_string(pos.z) + "\n").c_str());

	NetworkManager::GetOtherPlayer()->SetPosition(XMFLOAT3(pos.x, 0.0f, pos.z));
	NetworkManager::GetOtherPlayer()->SetRotateY((float)data.rotateY / (float)MAGNFICATION);

	return 1;
}

bool UdpClient::Recv(SOCKET sock, DATA* value)
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

bool UdpClient::Send(SOCKET sock, DATA value)
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
