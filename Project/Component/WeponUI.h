#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Font;
	class WeponBase;
	class WeponUI : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;

		void SetSelectWepon(std::weak_ptr<WeponBase> wepon);

	private:
		int m_bulletNum = 1;
		int m_maxBulletNum = 2;
		std::string m_weponName = "";
		std::weak_ptr<Font> m_pWeponNameFont;
		std::weak_ptr<Font> m_pWeponBulletNumFont;
		std::weak_ptr<WeponBase> m_pWepon;
		//SERIALIZE_COMPONENT();
	};
}

