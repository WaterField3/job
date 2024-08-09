#pragma once

#include"direct3d.h"

#include "SimpleMath.h"

namespace TMF
{
	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;
		virtual void OnInitialize();
		virtual void OnFinalize();
		virtual void OnUpdate();
		virtual void OnLateUpdate();
		virtual void OnDraw();
		virtual void OnDrawImGui();

		virtual bool GetIsUpdate();
	protected:
	};
}
