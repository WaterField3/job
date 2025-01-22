#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Font;
	class WeponUI : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

		void SetSelectWepon(std::weak_ptr<Component> wepon);

	private:
		int m_bulletNum = 1;
		int m_maxBulletNum = 2;
		std::string m_weponName = "";
		std::weak_ptr<Font> m_pWeponNameFont;
		std::weak_ptr<Font> m_pWeponBulletNumFont;
		std::weak_ptr<Component> m_pWepon;
		//SERIALIZE_COMPONENT();
	};
}

