#include "Camera.h"

using namespace DirectX;

void Camera::SetEye(DirectX::XMFLOAT3 eyePosition)
{
	eye = eyePosition;
}

void Camera::SetFocus(DirectX::XMFLOAT3 focusPosition)
{
	focus = focusPosition;
}

void Camera::Update()
{
	// XMFLOAT3����XMVECTOR�ɕϊ�����
	XMVECTOR vEye = XMVectorSet(eye.x, eye.y, eye.z, 0);
	XMVECTOR vFocus = XMVectorSet(focus.x, focus.y, focus.z, 0);
	XMVECTOR vUp = XMVectorSet(up.x, up.y, up.z, 0);

	// View�ϊ��s��쐬�̊֐����Ăяo��
	matrixView = XMMatrixLookAtLH(vEye, vFocus, vUp);
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return matrixView;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	static FXMMATRIX matrixProj3D = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45), 16.0f / 9.0f, 0.01f, 100.0f);

	// ���s���e�̍s��쐬
	// �����@�A�F�f���o���ʂ̉��c�̒���
	// �����B�C�F�f���o����Ԃ̉��s�i��O�ƍŉ��j
	static FXMMATRIX matrixProj2D = XMMatrixOrthographicLH(
				16.0f * 0.3f, 9.0f * 0.3f, 0.0f, 3.0f);

	if (projectionMode == _3D)
	{
		return matrixProj3D;
	}
	else
	{
		return matrixProj2D;
	}
}
