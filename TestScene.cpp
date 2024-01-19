#include "TestScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Input.h"

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{

	// WinSock初期化
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0)
	{
		return 1;
	}
	// リスンソケットの作成
	int listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, 0);	// 0で自動設定
	// リスンソケット作成失敗
	if (listenSock < 0)
	{
		// エラーコードを出力
		// 終了
		return 1;
	}

	// bind
	struct sockaddr_in bindAddr;	// bind用のソケットアドレス情報
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_port = htons(SERVERPORT);
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// ソケットアドレス情報設定	※固定のポート番号設定
	if (bind(listenSock, (struct sockaddr*)&bindAddr, sizeof(bindAddr)) != 0)
	{
		// エラーコードを出力
		// 終了
		return 1;
	}

	// リスン状態に設定	キューのサイズ:1
	if (listen(listenSock, 1) != 0)
	{
		// エラーコードを出力
		// 終了
		return 1;
	}

	// 通信用ソケット ( クライアントのソケットとこのソケット間にコネクションが確立 )
	int sock;


	struct sockaddr_in clientAddr;		// 接続要求をしてきたクライアントのソケットアドレス情報格納領域
	int addrlen = sizeof(clientAddr);	// clientAddrのサイズ

	// クライアントからのconnect()を受けて、コネクション確立済みのソケット作成
	sock = accept(listenSock, (struct sockaddr*)&clientAddr, &addrlen);
	if (sock < 0)
	{
		// エラーコードを出力
		// 終了
		return 1;
	}


}

//更新
void TestScene::Update()
{

	char buff[MESSAGELENGTH];	// 送受信メッセージの格納領域
	// クライアントからのメッセージ受信
	ret = recv(sock, buff, sizeof(buff) - 1, 0);
	if (ret < 0)
	{
		// ぬける
		break;
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
		break;
	}

}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
}
