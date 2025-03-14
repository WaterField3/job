#pragma once
#include "Component.h"

#include "memory"

namespace TMF
{
	class PlayerStatus;
	class SceneLoad;
	class Fade;
	class GameRule : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
	private:
		std::vector<std::weak_ptr<PlayerStatus>> m_pPlayerStatuses;
		std::weak_ptr<SceneLoad> m_pSceneLoad;
		std::weak_ptr<Fade> m_pFade;
	};
}

