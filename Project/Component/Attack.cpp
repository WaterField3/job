#include "Attack.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObjectManager.h"
#include "Input.h"
#include "Shot.h"
#include "Melee.h"
#include "MeleeFollowMove.h"
#include "WeponUI.h"

REGISTER_COMPONENT(TMF::Attack, "Attack");

namespace TMF
{
	void Attack::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pShots = pLockOwner->GetComponents<Shot>();
			for (auto pShot : pShots)
			{
				m_pWepons.push_back(pShot);
			}
			auto pMelees = pLockOwner->GetComponents<Melee>();
			for (auto pMelee : pMelees)
			{
				m_pWepons.push_back(pMelee);
			}
		}
		m_pCoolTimeUI = GameObjectManager::Instance().GetComponent<CoolTimeUI>();
		m_pChangeTimeUI = GameObjectManager::Instance().GetComponent<ChangeTimeUI>();
		m_pWeponUI = GameObjectManager::Instance().GetComponent<WeponUI>();
		m_pReloadUI = GameObjectManager::Instance().GetComponent<ReloadUI>();
		if (auto pLockWepon = m_pWepons[0].lock())
		{
			//SelectWeapon(pLockWepon);
			if (auto pLockCoolTimeUI = m_pCoolTimeUI.lock())
			{
				pLockCoolTimeUI->SetSelectWepon(pLockWepon);
			}
			if (auto pLockChangeTimeUI = m_pChangeTimeUI.lock())
			{
				pLockChangeTimeUI->SetSelectWepon(pLockWepon);
			}
			if (auto pLockWeponUI = m_pWeponUI.lock())
			{
				pLockWeponUI->SetSelectWepon(pLockWepon);
			}
			if (auto pLockReloadUI = m_pReloadUI.lock())
			{
				pLockReloadUI->SetSelectWepon(pLockWepon);
			}
		}
		m_previousScrollValue = 0;
	}
	void Attack::OnFinalize()
	{

	}
	void Attack::OnUpdate()
	{
		auto tracker = Input::Instance().GetMouseTracker();
		auto mouseState = Input::Instance().GetMouseState();
		tracker->Update(mouseState);
		//if (tracker->leftButton == DirectX::Mouse::ButtonStateTracker::ButtonState::PRESSED)
		//{
		//	Play();
		//}
		UpdateWeaponSelection(mouseState.scrollWheelValue);

	}
	float Attack::Play()
	{
		if (auto pLockSelectComponent = m_pWepons[m_selectIndex].lock())
		{
			// Shotクラスに変換できるか確認
			if (auto pLockShot = std::dynamic_pointer_cast<Shot>(pLockSelectComponent))
			{
				pLockShot->Play();
				if (auto pLockCoolTimeUI = m_pCoolTimeUI.lock())
				{
					pLockCoolTimeUI->SetSelectWepon(pLockShot);
					return 0.0f;
				}
				
			}
			// Meleeクラスに変換できるか確認
			else if (auto pLockMelee = std::dynamic_pointer_cast<Melee>(pLockSelectComponent))
			{
				pLockMelee->Play();
				if (auto pLockCoolTimeUI = m_pCoolTimeUI.lock())
				{
					pLockCoolTimeUI->SetSelectWepon(pLockMelee);
					return pLockMelee->GetEndTime();
				}
			}
		}
		return 0.0f;
	}
	void Attack::OnLateUpdate()
	{

	}
	void Attack::OnDraw()
	{

	}
	void Attack::OnDrawImGui()
	{
		auto selectIndexLabel = StringHelper::CreateLabel("SelectIndex", m_uuID);
		if (ImGui::SliderInt(selectIndexLabel.c_str(), &m_selectIndex, -100, 100))
		{

		}
	}
	void Attack::UpdateWeaponSelection(int currentScrollValue)
	{

		int scrollDelta = currentScrollValue - m_previousScrollValue;
		float scrollSensitivity = 0.001f;
		float adjustedScroll = scrollDelta * scrollSensitivity;

		if (adjustedScroll > 2 || adjustedScroll < -2)
		{
			m_previousScrollValue = 0;
			Input::Instance().ResetScrollWheelValue();
			return;
		}

		if (adjustedScroll != 0)
		{
			m_selectIndex = std::clamp(m_selectIndex + (adjustedScroll > 0 ? 1 : -1), 0, static_cast<int>(m_pWepons.size()) - 1);

			if (auto pLockSelectComponent = m_pWepons[m_selectIndex].lock())
			{
				HandleWeaponSelection(pLockSelectComponent);
			}
		}

		m_previousScrollValue = currentScrollValue;
		m_pOldWepon = m_pWepons[m_selectIndex];
	}
	void Attack::HandleWeaponSelection(const std::shared_ptr<Component>& pLockSelectComponent)
	{
		if (auto pLockShot = std::dynamic_pointer_cast<Shot>(pLockSelectComponent))
		{
			SelectWeapon(pLockShot);
		}
		else if (auto pLockMelee = std::dynamic_pointer_cast<Melee>(pLockSelectComponent))
		{
			if (pLockMelee->GetIsMelee() == false)
			{
				auto meleeObjectName = pLockMelee->GetMeleeObjectName();
				auto pMeleeObject = GameObjectManager::Instance().GetGameObject(meleeObjectName);
				if (auto pLockMeleeObjectOwner = pMeleeObject.lock())
				{
					auto pMeleeFollowMove = pLockMeleeObjectOwner->GetComponent<MeleeFollowMove>();
					if (auto pLockMeleeFollowMove = pMeleeFollowMove.lock())
					{
						pLockMeleeObjectOwner->SetActive(true);
						pLockMeleeFollowMove->SetIsEnable(true);
					}
				}
			}
			SelectWeapon(pLockMelee);
		}
	}
}