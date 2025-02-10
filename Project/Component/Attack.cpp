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
#include "WeponBase.h"

REGISTER_COMPONENT(TMF::Attack, "Attack");

namespace TMF
{
	void Attack::OnInitialize()
	{
		CheckWepons();
	}

	void Attack::OnFinalize()
	{

	}
	void Attack::OnUpdate()
	{
		auto keyState = Input::Instance().GetKeyState();
		auto keyTracker = Input::Instance().GetKeyboardTracker();
		auto mouseTracker = Input::Instance().GetMouseTracker();
		auto mouseState = Input::Instance().GetMouseState();
		mouseTracker->Update(mouseState);

		// 各キー状態を配列に格納
		std::array<bool, 9> keyStates =
		{
			keyTracker->pressed.D1,
			keyTracker->pressed.D2,
			keyTracker->pressed.D3,
			keyTracker->pressed.D4,
			keyTracker->pressed.D5,
			keyTracker->pressed.D6,
			keyTracker->pressed.D7,
			keyTracker->pressed.D8,
			keyTracker->pressed.D9
		};

		for (size_t i = 0; i < keyStates.size(); ++i)
		{
			if (keyStates[i] && m_pWepons.size() > i)
			{
				m_selectIndex = static_cast<int>(i);
				UpdateHandleWeaponSelection();
			}
		}

		UpdateWeaponSelection(mouseState.scrollWheelValue);

	}
	void Attack::UpdateHandleWeaponSelection()
	{
		if (auto pLockSelectComponent = m_pWepons[m_selectIndex].lock())
		{
			HandleWeaponSelection(pLockSelectComponent);
		}
	}
	float Attack::Play()
	{
		// 武器が無ければ行わない
		if (m_pWepons.size() == 0)
		{
			return 0.0f;
		}

		// WeponBaseを継承したクラスがあるか
		if (auto pLockSelectComponent = m_pWepons[m_selectIndex].lock())
		{
			// あれば攻撃を行う
			pLockSelectComponent->Play();
			if (auto pLockCoolTimeUI = m_pCoolTimeUI.lock())
			{
				// CoolTimeUIに使用したウェポンを設定
				pLockCoolTimeUI->SetSelectWepon(pLockSelectComponent);
				return pLockSelectComponent->GetEndTime();
			}
		}
		return 0.0f;
	}
	void Attack::WeponsUpdate()
	{
		CheckWepons();
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

		if (ImGui::Button("Check"))
		{
			CheckWepons();
		}
	}
	std::shared_ptr<Component> Attack::OnClone() const
	{
		auto pClone = std::make_shared<Attack>();
		return move(pClone);
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
		if (m_pWepons.size() > 0)
		{
			m_pOldWepon = m_pWepons[m_selectIndex];
		}
	}
	void Attack::HandleWeaponSelection(const std::shared_ptr<WeponBase>& pLockSelectComponent)
	{

		auto pWeponOwner = pLockSelectComponent->GetOwner();
		if (auto pLockWeponOwner = pWeponOwner.lock())
		{
			pLockWeponOwner->SetActive(true);
			SelectWeapon(pLockSelectComponent);
		}

	}
	void Attack::CheckWepons()
	{
		// 自身を保持しているゲームオブジェクトがあるか確認
		if (auto pLockOwner = m_pOwner.lock())
		{
			// 保持している武器をクリア
			m_pWepons.clear();
			// 自身の子オブジェクトを取得
			auto pChildren = pLockOwner->GetChildren();
			// 子オブジェクトの数分ループ
			for (auto& pChild : pChildren)
			{
				// 子オブジェクトがSharedに変換できるか確認
				if (auto pLockChild = pChild.lock())
				{
					// 子オブジェクトからWeponBaseを取得する
					auto pWepon = pLockChild->GetComponent<WeponBase>();
					// 取得したWeponBaseがSharedに変換できる(存在している)か確認
					if (auto pLockWepon = pWepon.lock())
					{
						// 武器として格納
						m_pWepons.push_back(pLockWepon);
					}
				}
			}
		}
		m_pCoolTimeUI = GameObjectManager::Instance().GetComponent<CoolTimeUI>();
		m_pChangeTimeUI = GameObjectManager::Instance().GetComponent<ChangeTimeUI>();
		m_pWeponUI = GameObjectManager::Instance().GetComponent<WeponInfoUI>();
		m_pReloadUI = GameObjectManager::Instance().GetComponent<ReloadUI>();
		if (m_pWepons.size() > 0)
		{
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
		}
		m_previousScrollValue = 0;
	}
}