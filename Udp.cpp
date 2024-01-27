#include "Udp.h"

namespace {
	// �|�[�g�ԍ�
	const unsigned short SERVERPORT = 10000;

	// ����M���郁�b�Z�[�W�̍ő�l
	const unsigned int MESSAGELENGTH = 1024;

}

bool Udp::Send(SOCKET sock, DATA value)
{
	//���M�f�[�^:�l�b�g���[�N�o�C�g�I�[�_�[�ɕϊ���̒l���i�[
	DATA sendValue;

	//�o�C�g�I�[�_�[�ϊ�
	sendValue.posX = htonl(value.posX);
	sendValue.posZ = htonl(value.posZ);
	sendValue.rotateY = htonl(value.rotateY);

	SOCKADDR_IN toAddr;	// ����̃\�P�b�g�A�h���X���
	memset(&toAddr, 0, sizeof(toAddr));
	toAddr.sin_family = AF_INET;				// IPv4�A�h���X
	toAddr.sin_port = htons(SERVERPORT);		// �T�[�o�̃|�[�g�ԍ�
	inet_pton(AF_INET, port_.c_str(), &toAddr.sin_addr.s_addr);		// �T�[�o��IP�A�h���X
	int ret = sendto(sock, (char*)&sendValue, sizeof(sendValue), 0, (SOCKADDR*)&toAddr, sizeof(toAddr));
	if (ret != sizeof(sendValue))
	{
		OutputDebugString("����Ȃ�����\n");
		return false;
	}

	return true;
}

bool Udp::Recv(SOCKET sock, DATA* value)
{
	DATA recvValue;	// ��M�f�[�^�̊i�[�̈�...�l�b�g���[�N�o�C�g�I�[�_�[���

	SOCKADDR_IN fromAddr;	// ���M���\�P�b�g�A�h���X�����i�[����̈�
	int fromlen = sizeof(fromAddr);
	int ret = recvfrom(sock, (char*)&recvValue, sizeof(recvValue), 0, (SOCKADDR*)&fromAddr, &fromlen);
	if (ret != sizeof(recvValue))
	{
		// ����ɑ���ꂽ���ǃf�[�^�͂Ȃ�
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			OutputDebugString("no Data\n");
			return true;
		}
		// �G���[
		else {
			OutputDebugString("Update Error\n");
			return false;
		}
	}

	// �������̏���
	value->posX = ntohl(recvValue.posX);
	value->posZ = ntohl(recvValue.posZ);
	value->rotateY = ntohl(recvValue.rotateY);
	
	return 1;
}