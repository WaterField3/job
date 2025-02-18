#include "Attack.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObjectManager.h"
#include "Input.h"
#include "Shot.h"
#include "Melee.h"
#include "MeleeFollowMove.h"
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

		// �e�L�[��Ԃ�z��Ɋi�[
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
		// �U���Ɋւ�鎞�Ԃ��쐬
		auto weaponActionTiming = WeaponActionTiming();
		// ���킪������΍s��Ȃ�
		if (m_pWeapons.size() == 0)
		{
			// �U���ɂ����鎞�Ԃ�Ԃ�
			return weaponActionTiming;
		}
		// ����I��p�̃C���f�b�N�X������̍ő吔�𒴂��Ă��邩
		if (m_selectIndex > m_pWeapons.size() - 1)
		{
			// �����Ă�����ő吔��
			m_selectIndex = int(m_pWeapons.size() - 1);
		}
		// ����I��p�̃C���f�b�N�X���}�C�i�X�̒l�ɂȂ��Ă��邩
		else if (m_selectIndex < 0)
		{
			// �}�C�i�X�ł����0��
			m_selectIndex = 0;
		}

		// WeaponBase���p�������N���X�����邩
		if (auto pLockSelectComponent = m_pWeapons[m_selectIndex].lock())
		{
			// ����΍U�����s��
			pLockSelectComponent->Play();
			weaponActionTiming.attackEndTiming = pLockSelectComponent->GetEndTime();
			weaponActionTiming.attackCancelTiming = pLockSelectComponent->GetCancelTime();
			if (auto pLockCoolTimeUI = m_pCoolTimeUI.lock())
			{
				// CoolTimeUI�Ɏg�p�����E�F�|����ݒ�
				pLockCoolTimeUI->SetSelectWeapon(pLockSelectComponent);
				return weaponActionTiming;
			}
		}
		// �U���ɂ����鎞�Ԃ�Ԃ�
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
				if (meleeTime != 0.0f)
				{
					late = pLockOldWeapon->GetEndTime() - meleeTime;
				}
				m_lateTime = late;
			}
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
		// ���g��ێ����Ă���Q�[���I�u�W�F�N�g�����邩�m�F
		if (auto pLockOwner = m_pOwner.lock())
		{
			// �ێ����Ă��镐����N���A
			m_pWeapons.clear();
			// ���g�̎q�I�u�W�F�N�g���擾
			auto pChildren = pLockOwner->GetChildren();
			// �q�I�u�W�F�N�g�̐������[�v
			for (auto& pChild : pChildren)
			{
				// �q�I�u�W�F�N�g��Shared�ɕϊ��ł��邩�m�F
				if (auto pLockChild = pChild.lock())
				{
					// �q�I�u�W�F�N�g����WeponBase���擾����
					auto pWepon = pLockChild->GetComponent<WeaponBase>();
					// �擾����WeponBase��Shared�ɕϊ��ł���(���݂��Ă���)���m�F
					if (auto pLockWepon = pWepon.lock())
					{
						// ����Ƃ��Ċi�[
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
}