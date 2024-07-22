#pragma once

#include "Layer.h"


namespace TMF
{
	class TestLayer : public Layer
	{
		//TestLayer(const char name);
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnDraw() override;
		void SetName(const char name[64]) override;
		const DirectX::XMFLOAT3 GetTransform(const char name[32]) override;
	public:
		const char* GetName() override;
	private:
	};
}

