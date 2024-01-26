#pragma once
#include <string>

namespace NetworkManager {

	//�쐬����\�P�b�g�̎�ނ̎w��Ɏg�p
	enum SOCKET_MODE {
		UDP_SERVER,	//�A�N�Z�X���Ă�������𑗐M��Ɏ����ݒ肳��A�����l�ɑ��M���s��
		UDP_CLIENT,	//�C�ӂɐݒ肵�������l�݂̂ɑ��M���s��
	};

	//����������
	int Initialize();

	int CreateSocket(SOCKET_MODE mode, std::string port);

	int Update();

}
