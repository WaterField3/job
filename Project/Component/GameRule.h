#pragma once
#include "Component.h"

#include "memory"

namespace TMF
{
	class PlayerStatus;
	class SceneLoad;
	class GameRule : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
	private:
		std::vector<std::weak_ptr<PlayerStatus>> m_pPlayerStatuses;
		std::weak_ptr<SceneLoad> m_pSceneLoad;
	};
}

