#include "UdpClient.h"
#include <WS2tcpip.h>

namespace {
	// �|�[�g�ԍ�
	const unsigned short SERVERPORT = 8888;
	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;

	struct DATA {
		double posX;
		double posZ;
	};

}

bool Recv(int sock, DATA* value)
{
	DATA recvValue;	// ��M�f�[�^�̊i�[�̈�...�l�b�g���[�N�o�C�g�I�[�_�[���
	int ret;		// ���ۂ̔���p

	// ��M
	ret = recv(sock, (char*)&recvValue, sizeof(recvValue), 0);
	// ���s
	if (ret != sizeof(recvValue))
	{
		//����ɑ���ꂽ���ǃf�[�^�͂Ȃ�
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			OutputDebugString("no Data\n");
			return true;
		}
		//�G���[
		else {
			OutputDebugString(WSAGetLastError() + " : Error\n");
			return false;
		}
	}

	// �������̏���
	value->posX = ntohl(recvValue.posX);								// int �o�C�g�I�[�_�[�ϊ�
	value->posZ = ntohl(recvValue.posZ);								// int �o�C�g�I�[�_�[�ϊ�
	return true;
}

bool Send(int sock, DATA value)
{
	DATA sendValue;	// ���M�f�[�^ ... �l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ���̒l���i�[
	sendValue.posX = htonl(value.posX);									// int �o�C�g�I�[�_�[�ϊ�
	sendValue.posZ = htonl(value.posZ);									// int �o�C�g�I�[�_�[�ϊ�

	int ret;		// ���ۂ̔���p
	// ���M
	ret = send(sock, (char*)&sendValue, sizeof(sendValue), 0);
	// ���s
	if (ret != sizeof(sendValue))
	{
		return false;
	}

	// ����
	return true;
}

int UdpClient::CreateSocket(std::string port)
{
	// �\�P�b�g�̍쐬
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock < 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 1;
	}

	// �ڑ���T�[�o�̃\�P�b�g�A�h���X���ݒ�
	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	inet_pton(AF_INET, port.c_str(), &serverAddr.sin_addr.s_addr);

	// �ڑ��v��
	if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
	{
		OutputDebugString(WSAGetLastError() + " : Error\n");
		return 1;
	}

	// �\�P�b�gsock���m���u���b�L���O�\�P�b�g�ɂ���
	unsigned long cmdarg = 0x01;
	int ret = ioctlsocket(sock, FIONBIO, &cmdarg);

	if (ret == SOCKET_ERROR)
	{
		// �G���[����
	}

}

int UdpClient::Update()
{
	DATA data;
	data.posX = 10.0f;
	data.posZ = -5.0f;

	// ���M
	if (!Send(sock, data)) {
		return -1;
	}
	
	//��M
	if (!Recv(sock, &data)) {
		return -1;
	}

	// �o��
	OutputDebugString("X = ");
	OutputDebugStringA(std::to_string(data.posX).c_str());
	OutputDebugString(" : Y = ");
	OutputDebugStringA(std::to_string(data.posZ).c_str());
	OutputDebugString("\n");


}
