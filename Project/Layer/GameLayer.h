#pragma once

#include "Layer.h"

namespace TMF
{
	class GameLayer : public Layer
	{
	private:

		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;

	};
}

