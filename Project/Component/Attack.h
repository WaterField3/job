#pragma once
#include "Component.h"

#include "ChangeTimeUI.h"
#include "CoolTimeUI.h"
#include "WeaponInfoUI.h"
#include "ReloadUI.h"

namespace TMF
{
	class Melee;
	class Shot;
	class WeaponBase;
	class Attack : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		/// <summary>
		/// UŒ‚
		/// </summary>
		/// <returns>UŒ‚‚ÌŠÔ ¦ËŒ‚‚Í‚O</returns>
		float Play();
		void WeponsUpdate();
	private:
		void UpdateHandleWeaponSelection();
		void UpdateWeaponSelection(int currentScrollValue);
		void HandleWeaponSelection(const std::shared_ptr<WeaponBase>& pLockSelectComponent);
		void CheckWepons();

		template <typename T>
		void SelectWeapon(const std::shared_ptr<T>& wepon)
		{
			if (wepon != m_pOldWepon.lock())
			{
				wepon->Select();
			}

			if (auto pLockChangeTimeUI = m_pChangeTimeUI.lock())
			{
				pLockChangeTimeUI->SetSelectWepon(wepon);
			}

			if (auto pLockCoolTimeUI = m_pCoolTimeUI.lock())
			{
				pLockCoolTimeUI->SetSelectWepon(wepon);
			}
			if (auto pLockWeponUI = m_pWeaponUI.lock())
			{
				pLockWeponUI->SetSelectWepon(wepon);
			}
			if (auto pLockReloadUI = m_pReloadUI.lock())
			{
				pLockReloadUI->SetSelectWepon(wepon);
			}
		}

	private:
		int m_previousScrollValue = 0;
		int m_selectIndex = 0;
		// •Ší‚ğŠi”[‚·‚é•Ï”
		std::vector<std::weak_ptr<WeaponBase>> m_pWeapons;

		std::weak_ptr<WeaponBase> m_pOldWepon;
		std::weak_ptr<CoolTimeUI> m_pCoolTimeUI;
		std::weak_ptr<ChangeTimeUI> m_pChangeTimeUI;
		std::weak_ptr<WeaponInfoUI> m_pWeaponUI;
		std::weak_ptr<ReloadUI> m_pReloadUI;
	};
}

