#pragma once

#include "direct3d.h"

#include "Layer.h"

namespace TMF
{
	class CameraLayer : public Layer
	{
	public:
		//CameraLayer(const char name);
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnDraw() override;

		void SetEyePos(DirectX::XMFLOAT3 _eyePos);
		void SetFocus(DirectX::XMFLOAT3 _focus);

		void SetCameraOwner(Layer* _owner);
		DirectX::XMMATRIX GetViewMatrix();
		DirectX::XMMATRIX GetProjectionMatrix();


	private:

		Layer* owner;

		// �v�Z�ς݃r���[�ϊ��s���ۑ�����ϐ�
		DirectX::XMMATRIX matrixView;

		DirectX::XMMATRIX matrixProj;

		DirectX::XMFLOAT3 initEye = { 0.0f,0.0f,-1.0f };

		DirectX::XMFLOAT3 distance = {0.0f,1.0f,-1.0f};

		DirectX::XMFLOAT3 rotation{  0.0f,90.0f,0.0f };
		// Eye��ۑ�����ϐ�
		DirectX::XMFLOAT3 eye = initEye;
		// Focus��ۑ�����ϐ�
		DirectX::XMFLOAT3 focus = { 0,0,-1.0 };
		// Up��ۑ�����ϐ�
		DirectX::XMFLOAT3 up = { 0,1,0 };
	};
}

