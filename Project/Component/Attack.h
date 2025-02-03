#pragma once
#include "Component.h"

#include "ChangeTimeUI.h"
#include "CoolTimeUI.h"
#include "WeponUI.h"
#include "ReloadUI.h"

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
		std::shared_ptr<Component> OnClone() const override;
		/// <summary>
		/// çUåÇ
		/// </summary>
		/// <returns>çUåÇéûÇÃéûä‘ Å¶éÀåÇéûÇÕÇO</returns>
		float Play();
	private:
		void UpdateHandleWeaponSelection();
		void UpdateWeaponSelection(int currentScrollValue);
		void HandleWeaponSelection(const std::shared_ptr<Component>& pLockSelectComponent);

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
			if (auto pLockWeponUI = m_pWeponUI.lock())
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
		std::vector<std::weak_ptr<Component>> m_pWepons;
		std::weak_ptr<Component> m_pOldWepon;
		std::weak_ptr<CoolTimeUI> m_pCoolTimeUI;
		std::weak_ptr<ChangeTimeUI> m_pChangeTimeUI;
		std::weak_ptr<WeponUI> m_pWeponUI;
		std::weak_ptr<ReloadUI> m_pReloadUI;
	};
}

