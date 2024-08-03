#pragma once
#include "Layer.h"

namespace TMF
{
	class PhysicsLayer : public Layer
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate()override;
		void OnDraw() override;

	private:
	};
}
