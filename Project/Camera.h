#pragma once

#include "direct3d.h"

// �J�����̎��_�𑀍삷��@�\�����N���X
class Camera
{

public:
	// �J���������݂�����W��ݒ肷��֐�
	void SetEye(DirectX::XMFLOAT3 eyePosition);

	// �J�������_�����W��ݒ肷��֐�
	void SetFocus(DirectX::XMFLOAT3 focusPosition);

	// �r���[�ϊ��s����v�Z����֐�
	void Update();

	// �v�Z�ς݃r���[�ϊ��s����擾����֐�
	DirectX::XMMATRIX GetViewMatrix();

	DirectX::XMMATRIX GetProjectionMatrix();

	enum PROJECTION_MODE
	{
		_3D, _2D
	};

	void SetProjectionMode(PROJECTION_MODE mode) { projectionMode = mode; }

private:
	// �����o�[�ϐ�
	// �v�Z�ς݃r���[�ϊ��s���ۑ�����ϐ�
	DirectX::XMMATRIX matrixView;

	//DirectX::XMMATRIX matrixProj;

	// Eye��ۑ�����ϐ�
	DirectX::XMFLOAT3 eye = { 0,0,-1 };
	// Focus��ۑ�����ϐ�
	DirectX::XMFLOAT3 focus = { 0,0,0 };
	// Up��ۑ�����ϐ�
	DirectX::XMFLOAT3 up = { 0,1,0 };

	PROJECTION_MODE projectionMode = _3D; // �f�t�H���g��3D �� �������e

};

