#pragma once
#include "UIBase.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class PlayerStatus;
	class Font;
	class HPUI : public UIBase
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
		float m_maxHP = 1.0f;
		std::string m_targetName = "";
		std::weak_ptr<PlayerStatus> m_pPlayerStatus;
		std::weak_ptr<Font> m_pFont;
		
		SERIALIZE_COMPONENT(m_barWidth, m_barHeight, m_barTextureName, m_drawPosition, m_targetName);

	};
}

