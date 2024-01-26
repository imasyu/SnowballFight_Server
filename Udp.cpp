#include "Udp.h"

bool Udp::Recv(int sock, DATA* value)
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

bool Udp::Send(int sock, DATA value)
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
