#include "Attack.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Shot.h"
#include "Melee.h"
#include "Input.h"

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
	}
	void Attack::OnFinalize()
	{

	}
	void Attack::OnUpdate()
	{
		auto tracker = Input::Instance().GetMouseTracker();
		auto mouseState = Input::Instance().GetMouseState();
		tracker->Update(mouseState);
		if (tracker->leftButton == DirectX::Mouse::ButtonStateTracker::ButtonState::PRESSED)
		{
			if (auto pLockSelectComponent = m_pWepons[m_selectIndex].lock())
			{
				// Shotクラスに変換できるか確認
				if (auto pLockShot = std::dynamic_pointer_cast<Shot>(pLockSelectComponent))
				{
					pLockShot->Play();

				}
				// Meleeクラスに変換できるか確認
				else if (auto pLockMelee = std::dynamic_pointer_cast<Melee>(pLockSelectComponent))
				{
					pLockMelee->Play();
				}
			}
		}
		int currentScrollValue = mouseState.scrollWheelValue;
		int scrollDelta = currentScrollValue - m_previousScrollValue;
		float scrollSensitivity = 0.001f;
		float adjustedScroll = scrollDelta * scrollSensitivity;
		if (adjustedScroll > 2 || adjustedScroll < -2)
		{
			m_previousScrollValue = 0;
		}
		if (adjustedScroll > 0)
		{
			m_selectIndex++;
			auto maxSize = m_pWepons.size();
			if (m_selectIndex >= maxSize)
			{
				m_selectIndex = int(maxSize) - 1;
			}
		}
		else if (adjustedScroll < 0)
		{
			m_selectIndex--;
			if (m_selectIndex <= -1)
			{
				m_selectIndex = 0;
			}
		}
		m_previousScrollValue = currentScrollValue;
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
}