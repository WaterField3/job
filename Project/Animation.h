#pragma once

#include "direct3d.h"

// �A�j���[�V�����@�\�̃N���X
class Animation
{

public:
	// �񋓌^
	// �A�j���[�V�����p�^�[����
	enum PATTERN
	{
		NO_ANIMATION,
		FX_0_11 = 5  // �G�t�F�N�g�p�O�`�P�P�Đ�
	};

	// �����o�[�ϐ�
	DirectX::XMFLOAT2 uv; // �A�j���[�V������UV�ړ��ʂ�ۑ�

	float animCounter = 0.0f; // �A�j���e�[�u���̃J�E���^�[

	// �Đ�����p�^�[��ID�����ϐ�
	int animPattern = 1;

	

public:
	// �Đ����x��ݒ肷��
	float animSpeed = 0.1f; // �Đ����x

	bool isPlaying = true;

public:
	// �����o�[�֐�
	void Update();

	// �Đ�����A�j���[�V������ύX����

	// UV�ړ��ʂ��擾����
	DirectX::XMFLOAT2 GetUV();

	// �Đ�����p�^�[����ς���
	void SetPattern(int pattern);

	bool IsPlaying() { return isPlaying; }

};

