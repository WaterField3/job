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
		virtual void SetName(const char name[64]);
		virtual const char* GetName();
		virtual const DirectX::XMFLOAT3 GetTransform(const char name[32]);
		DirectX::XMFLOAT3 GetForward();
	protected:
	 const char *m_name;

		D3D::Model model;
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT3 m_scale;
		DirectX::XMFLOAT3 m_rotation;

		DirectX::XMFLOAT3 forward = { 0.0f,0.0f,1.0f };

	protected:
	};
}

