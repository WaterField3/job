#pragma once
#include "Component.h"

#include "ChangeTimeUI.h"
#include "CoolTimeUI.h"

namespace TMF
{
	class Melee;
	class Shot;
	class Attack : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
	private:
		void UpdateWeaponSelection(int currentScrollValue);
		void HandleWeaponSelection(const std::shared_ptr<Component>& pLockSelectComponent);

		template <typename T>
		void SelectWeapon(const std::shared_ptr<T>& weapon)
		{
			if (weapon != m_pOldWepon.lock())
			{
				weapon->Select();
			}

			if (auto pLockChangeTimeUI = m_pChangeTimeUI.lock())
			{
				pLockChangeTimeUI->SetSelectWepon(weapon);
			}

			if (auto pLockCoolTimeUI = m_pCoolTimeUI.lock())
			{
				pLockCoolTimeUI->SetSelectWepon(weapon);
			}
		}

	private:
		int m_previousScrollValue = 0;
		int m_selectIndex = 0;
		std::vector<std::weak_ptr<Component>> m_pWepons;
		std::weak_ptr<Component> m_pOldWepon;
		std::weak_ptr<CoolTimeUI> m_pCoolTimeUI;
		std::weak_ptr<ChangeTimeUI> m_pChangeTimeUI;
	};
}

