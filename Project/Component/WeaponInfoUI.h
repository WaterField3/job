#pragma once
#include "WeaponUI.h"

namespace TMF
{

	class WeaponInfoUI : public WeaponUI
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void OnSetSelectWeapon(std::weak_ptr<WeaponBase> pWeapon) override;

	private:
		int m_bulletNum = 1;
		int m_maxBulletNum = 2;
		std::string m_weaponName = "";
		std::weak_ptr<Font> m_pWeaponNameFont;
		std::weak_ptr<Font> m_pWeaponBulletNumFont;

	};
}

