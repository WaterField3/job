#pragma once

#include"direct3d.h"

#include "SimpleMath.h"

namespace TMF
{
	class Layer
	{
	public:
		//Layer(const char* name);
		virtual void OnInitialize();
		virtual void OnFinalize();
		virtual void OnUpdate();
		virtual void OnDraw();

	protected:

		//D3D::Model model;
		//DirectX::XMFLOAT3 m_position;
		//DirectX::XMFLOAT3 m_scale;
		//DirectX::XMFLOAT3 m_rotation;

		//DirectX::XMFLOAT3 forward = { 0.0f,0.0f,1.0f };

	protected:
	};
}

