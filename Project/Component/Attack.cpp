#include "Attack.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObjectManager.h"
#include "Input.h"
#include "WeaponUI.h"
#include "WeaponBase.h"

REGISTER_COMPONENT(TMF::Attack, "Attack");

namespace TMF
{
	void Attack::OnInitialize()
	{
		CheckWeapons();
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
			if (keyStates[i] && m_pWeapons.size() > i)
			{
				if (auto pLockWepon = m_pWeapons[m_selectIndex].lock())
				{
					m_lateTime = pLockWepon->GetMeleeTime();
				}
				m_selectIndex = static_cast<int>(i);
				UpdateHandleWeaponSelection();
			}
		}

		UpdateWeaponSelection(mouseState.scrollWheelValue);

	}
	void Attack::UpdateHandleWeaponSelection()
	{
		if (auto pLockSelectComponent = m_pWeapons[m_selectIndex].lock())
		{
			HandleWeaponSelection(pLockSelectComponent);
		}
	}
	Attack::WeaponActionTiming Attack::Play()
	{
		// 攻撃に関わる時間を作成
		auto weaponActionTiming = WeaponActionTiming();
		// 武器が無ければ行わない
		if (m_pWeapons.size() == 0)
		{
			// 攻撃にかかる時間を返す
			return weaponActionTiming;
		}
		// 武器選択用のインデックスが武器の最大数を超えているか
		if (m_selectIndex > m_pWeapons.size() - 1)
		{
			// 超えていたら最大数に
			m_selectIndex = int(m_pWeapons.size() - 1);
		}
		// 武器選択用のインデックスがマイナスの値になっているか
		else if (m_selectIndex < 0)
		{
			// マイナスであれば0に
			m_selectIndex = 0;
		}

		// WeaponBaseを継承したクラスがあるか
		if (auto pLockSelectComponent = m_pWeapons[m_selectIndex].lock())
		{
			// あれば攻撃を行う
			auto isAttack = pLockSelectComponent->Play();
			if (isAttack == true)
			{
				weaponActionTiming.attackEndTiming = pLockSelectComponent->GetEndTime() + pLockSelectComponent->GetMeleeStopTime();
				weaponActionTiming.attackCancelTiming = pLockSelectComponent->GetCancelTime();
			}
			if (auto pLockCoolTimeUI = m_pCoolTimeUI.lock())
			{
				// CoolTimeUIに使用したウェポンを設定
				pLockCoolTimeUI->SetSelectWeapon(pLockSelectComponent);
				return weaponActionTiming;
			}
		}
		// 攻撃にかかる時間を返す
		return weaponActionTiming;
	}
	void Attack::WeaponsUpdate()
	{
		CheckWeapons();
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
			CheckWeapons();
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
			if (auto pLockCurrentComponent = m_pWeapons[m_selectIndex].lock())
			{
				m_lateTime = pLockCurrentComponent->GetMeleeTime();
			}

			m_selectIndex = std::clamp(m_selectIndex + (adjustedScroll > 0 ? 1 : -1), 0, static_cast<int>(m_pWeapons.size()) - 1);

			if (auto pLockSelectComponent = m_pWeapons[m_selectIndex].lock())
			{
				HandleWeaponSelection(pLockSelectComponent);
			}
		}

		m_previousScrollValue = currentScrollValue;
		if (m_pWeapons.size() > 0)
		{
			m_pOldWepon = m_pWeapons[m_selectIndex];
		}
	}
	void Attack::HandleWeaponSelection(const std::shared_ptr<WeaponBase>& pLockSelectComponent)
	{
		auto late = 0.0f;
		if (pLockSelectComponent != m_pOldWepon.lock())
		{
			if (auto pLockOldWeapon = m_pOldWepon.lock())
			{
				auto meleeTime = pLockOldWeapon->GetMeleeTime();
				if (meleeTime != 0.0f && pLockOldWeapon->GetIsCancel() == false)
				{
					late = pLockOldWeapon->GetEndTime();
				}
			}
			m_lateTime = late;
		}
		pLockSelectComponent->SetLateTimer(late);
		auto pWeponOwner = pLockSelectComponent->GetOwner();
		if (auto pLockWeponOwner = pWeponOwner.lock())
		{
			pLockWeponOwner->SetActive(true);
			SelectWeapon(pLockSelectComponent);
		}

	}
	void Attack::CheckWeapons()
	{
		// 自身を保持しているゲームオブジェクトがあるか確認
		if (auto pLockOwner = m_pOwner.lock())
		{
			// 保持している武器をクリア
			m_pWeapons.clear();
			// 自身の子オブジェクトを取得
			auto pChildren = pLockOwner->GetChildren();
			// 子オブジェクトの数分ループ
			for (auto& pChild : pChildren)
			{
				// 子オブジェクトがSharedに変換できるか確認
				if (auto pLockChild = pChild.lock())
				{
					// 子オブジェクトからWeponBaseを取得する
					auto pWepon = pLockChild->GetComponent<WeaponBase>();
					// 取得したWeponBaseがSharedに変換できる(存在している)か確認
					if (auto pLockWepon = pWepon.lock())
					{
						// 武器として格納
						m_pWeapons.push_back(pLockWepon);
					}
				}
			}
		}
		m_pCoolTimeUI = GameObjectManager::Instance().GetComponent<CoolTimeUI>();
		m_pChangeTimeUI = GameObjectManager::Instance().GetComponent<ChangeTimeUI>();
		m_pWeaponUI = GameObjectManager::Instance().GetComponent<WeaponInfoUI>();
		m_pReloadUI = GameObjectManager::Instance().GetComponent<ReloadUI>();
		if (m_pWeapons.size() > 0)
		{
			if (auto pLockWepon = m_pWeapons[0].lock())
			{
				//SelectWeapon(pLockWepon);
				if (auto pLockCoolTimeUI = m_pCoolTimeUI.lock())
				{
					pLockCoolTimeUI->SetSelectWeapon(pLockWepon);
				}
				if (auto pLockChangeTimeUI = m_pChangeTimeUI.lock())
				{
					pLockChangeTimeUI->SetSelectWeapon(pLockWepon);
				}
				if (auto pLockWeponUI = m_pWeaponUI.lock())
				{
					pLockWeponUI->SetSelectWeapon(pLockWepon);
				}
				if (auto pLockReloadUI = m_pReloadUI.lock())
				{
					pLockReloadUI->SetSelectWeapon(pLockWepon);
				}
			}
		}
		m_previousScrollValue = 0;
	}
	void Attack::CancelWepons()
	{
		auto size = m_pWeapons.size();
		if (size > 0)
		{
			for (auto i = 0; i < size; i++)
			{
				if (auto pLockWepon = m_pWeapons[i].lock())
				{
					pLockWepon->Cancel();
				}
			}
		}
		if (auto pLockWeapon = m_pWeapons[m_selectIndex].lock())
		{
			pLockWeapon->SetLateTimer(0.0f);
		}
	}
}