#pragma once

#include "Layer/Layer.h"
#include "GameObjectManager/GameObjectManager.h"

namespace TMF
{
	class GameLayer : public Layer
	{
	private:

		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate()override;
		void OnDraw() override;
		
	};
}

