#include "UdpServer.h"
#include <WS2tcpip.h>
#include "Player.h"
#include "NetworkManager.h"
#include "Engine/Input.h"

namespace {
	// ポート番号
	const unsigned short SERVERPORT = 8888;
	// 送受信するメッセージの最大値
	const unsigned int MESSAGELENGTH = 1024;

}

bool UdpServer::CreateSocket(std::string port)
{
	port_ = port;

	// リスンソケットの作成
	sock_ = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_ == INVALID_SOCKET)
	{
		OutputDebugString("リスンソケット作成失敗\n");
		return false;
	}

	// bind
	struct sockaddr_in bindAddr;	// bind用のソケットアドレス情報
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//ソケットアドレス情報設定	※固定のポート番号設定
	ret_ = bind(sock_, (SOCKADDR*)&bindAddr, sizeof(bindAddr));
	if (ret_ == SOCKET_ERROR)
	{
		OutputDebugString("ソケットアドレスの設定エラー\n");
		closesocket(sock_);  // ソケットを閉じる
		return false;
	}

	// ソケットsockをノンブロッキングソケットにする
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock_, FIONBIO, &cmdarg);
	if (ret == SOCKET_ERROR)
	{
		OutputDebugString("ノンブロッキングソケット化失敗\n");
		closesocket(sock_);  // ソケットを閉じる
		return false;
	}

	return true;
}

bool UdpServer::Update()
{
	//受信
	DATA data;
	if (Recv(sock_, &data)) {
		//受信できた
		XMFLOAT3 pos = { (float)data.posX / (float)MAGNFICATION, 0.0f, (float)data.posZ / (float)MAGNFICATION };
		//OutputDebugString(("X = " + std::to_string(pos.x) + " : Y = " + std::to_string(pos.z) + "\n").c_str());
		//OutputDebugString((std::to_string(data.shot) + "\n").c_str());

		if (data.shot == 0 || data.shot == 1) {
			float rotate = (float)data.rotateY / (float)MAGNFICATION;
			NetworkManager::GetOtherPlayer()->NotPlayerSetPosition(XMFLOAT3(pos.x, 0.0f, pos.z));
			NetworkManager::GetOtherPlayer()->SetRotateY(rotate);
			if (data.shot) NetworkManager::GetOtherPlayer()->Shot();
		}

	}
	else {
		OutputDebugString("受信エラー\n");
		return false;
	}

	//送信
	XMFLOAT3 pos = NetworkManager::GetSelfPlayer()->GetPosition();
	data.posX = (pos.x * MAGNFICATION);
	data.posZ = (pos.z * MAGNFICATION);
	data.rotateY = (NetworkManager::GetSelfPlayer()->GetRotate().y * MAGNFICATION);
	data.shot = Input::IsKeyDown(DIK_SPACE);

	if (!Send(sock_, data)) {
		OutputDebugString("送信エラー\n");
		return false;
	}

	return true;
}

bool UdpServer::Recv(SOCKET sock, DATA* value)
{
	DATA recvValue;	// 受信データの格納領域...ネットワークバイトオーダー状態

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
	value->shot = htons(recvValue.shot);

	return 1;
}

bool UdpServer::Send(SOCKET sock, DATA value)
{
	//送信データ:ネットワークバイトオーダーに変換後の値を格納
	DATA sendValue;

	//バイトオーダー変換
	sendValue.posX = htonl(value.posX);
	sendValue.posZ = htonl(value.posZ);
	sendValue.rotateY = htonl(value.rotateY);
	sendValue.shot = htons(value.shot);

	int fromlen = sizeof(fromAddr);
	ret_ = sendto(sock, (char*)&sendValue, sizeof(sendValue), 0, (SOCKADDR*)&fromAddr, fromlen);
	if (ret_ != sizeof(sendValue))
	{
		OutputDebugString("送れなかった\n");
		return false;
	}

	return true;
}