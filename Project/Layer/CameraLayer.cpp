#include "CameraLayer.h"
#include "Input.h"

using namespace DirectX;

//TMF::CameraLayer::CameraLayer(const char name)
//	: TMF::Layer(name)
//{
//}

void TMF::CameraLayer::OnInitialize()
{
	
}

void TMF::CameraLayer::OnFinalize()
{

}

void TMF::CameraLayer::OnUpdate()
{

	//XMFLOAT3 pos = owner->GetTransform("position");

	//XMFLOAT3 backforward = owner->GetForward();


	//backforward.x = backforward.x * -1;
	//backforward.y = backforward.y * -1;
	//backforward.z = backforward.z * -1;

	//focus.x = pos.x + backforward.x * distance.x;
	//focus.y = pos.y + backforward.y * distance.y;
	//focus.z = pos.z + backforward.z * distance.z;

	//eye = pos;

	////focus = 

	//rotation.x = owner->GetTransform("rotation").y;
	//rotation.y = owner->GetTransform("rotation").x;
	//rotation.z = owner->GetTransform("rotation").z;


	//DirectX::XMVECTOR vCameraPos
	//	= DirectX::XMVectorSet(initEye.x, initEye.y, initEye.z, 0);
	//// 回転行列を作る
	//DirectX::XMMATRIX matrixRotateY
	//	= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotation.y));
	//// カメラ初期位置と回転行列を掛ける
	//vCameraPos = DirectX::XMVector3TransformCoord(vCameraPos, matrixRotateY);
	//
	////eye.x = DirectX::XMVectorGetX(vCameraPos);
	////eye.y = DirectX::XMVectorGetY(vCameraPos);
	////eye.z = DirectX::XMVectorGetZ(vCameraPos);
	//
	////eye = pos;

	//// XMFLOAT3からXMVECTORに変換する
	//DirectX::XMVECTOR vEye = DirectX::XMVectorSet(eye.x, eye.y, eye.z, 0);
	//DirectX::XMVECTOR vFocus = DirectX::XMVectorSet(focus.x, focus.y, focus.z, 0);
	//DirectX::XMVECTOR vUp = DirectX::XMVectorSet(up.x, up.y, up.z, 0);

	//// View変換行列作成の関数を呼び出す
	//matrixView = DirectX::XMMatrixLookAtLH(vEye, vFocus, vUp);
}

void TMF::CameraLayer::OnDraw()
{

}

void TMF::CameraLayer::SetEyePos(DirectX::XMFLOAT3 _eyePos)
{

}

void TMF::CameraLayer::SetFocus(DirectX::XMFLOAT3 _focus)
{

}

void TMF::CameraLayer::SetCameraOwner(Layer* _owner)
{
	owner = _owner;
}

DirectX::XMMATRIX TMF::CameraLayer::GetViewMatrix()
{
	return matrixView;
}

DirectX::XMMATRIX TMF::CameraLayer::GetProjectionMatrix()
{
	matrixProj = DirectX::XMMatrixPerspectiveFovLH(
		DirectX::XMConvertToRadians(45), 16.0f / 9.0f, 0.01f, 100.0f);
	return matrixProj;
}
