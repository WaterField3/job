#pragma once

// Direct Input���g���ăL�[�{�[�h�ƃ}�E�X������擾����N���X
#define DIRECTINPUT_VERSION 0x0800
#include	<dinput.h>

class Input
{

private:
	IDirectInput8* mDInput = nullptr;
	IDirectInputDevice8* mDevKeyboad = nullptr;
	IDirectInputDevice8* mDevMouse = nullptr;

	unsigned char mKeyBuffer[256] = {};
	unsigned char mOldKeyBuffer[256] = {};

	DIMOUSESTATE2 mMouseState = {};
	DIMOUSESTATE2 mOldMouseState = {};

	Input() {};

	~Input();
	

public:
	// �V���O���g���̃C���X�^���X���擾����֐�
	static Input* Get();

	// DirectInput�̏��������s���֐�
	// �Q�[�����[�v���n�܂�O�ɌĂяo��
	void Initialize(HWND hWnd, HINSTANCE hInstance);

	// �����[�v�Ăяo���L�[�̉�����Ԃ��o�b�t�@�ɕۑ�����
	void Update();

	// ���݂̃L�[�{�[�h�̉�����Ԃ��擾����
	// key: DIK_�Ŏn�܂�L�[�萔���g��
	bool GetKeyPress(int key);

	bool GetKeyTrigger(int key);

	// �}�E�X�{�^���̉�����Ԃ��擾����
	// key: 0-4
	bool GetMousePress(int key);

	// �}�E�X�̈ړ��ʂ��擾����
	POINT GetMouseMove();

};

