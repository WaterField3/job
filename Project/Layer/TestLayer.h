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
	public:
	private:
	};
}

