#pragma once
#include "WeponUI.h"

namespace TMF
{

	class WeponInfoUI : public WeponUI
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void OnSetSelectWepon(std::weak_ptr<WeponBase> pWepon) override;

	private:
		int m_bulletNum = 1;
		int m_maxBulletNum = 2;
		std::string m_weponName = "";
		std::weak_ptr<Font> m_pWeponNameFont;
		std::weak_ptr<Font> m_pWeponBulletNumFont;

	};
}

