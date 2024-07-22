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
	// XMFLOAT3からXMVECTORに変換する
	XMVECTOR vEye = XMVectorSet(eye.x, eye.y, eye.z, 0);
	XMVECTOR vFocus = XMVectorSet(focus.x, focus.y, focus.z, 0);
	XMVECTOR vUp = XMVectorSet(up.x, up.y, up.z, 0);

	// View変換行列作成の関数を呼び出す
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

	// 平行投影の行列作成
	// 引数①②：映し出す面の横縦の長さ
	// 引数③④：映し出す空間の奥行（手前と最奥）
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
