#pragma once
#include <memory>
#include <string>

namespace NetworkManager {

	//�쐬����\�P�b�g�̎�ނ̎w��Ɏg�p
	enum class SOCKET_MODE : unsigned char {
		UDP_SERVER,	//�A�N�Z�X���Ă�������𑗐M��Ɏ����ݒ肳��A�����l�ɑ��M���s��
		UDP_CLIENT,	//�C�ӂɐݒ肵�������l�݂̂ɑ��M���s��
	};

	//����������
	int Initialize();

	int CreateSocket(SOCKET_MODE mode, unsigned short port);

	int Update();

}
