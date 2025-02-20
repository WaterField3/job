#include "Sword.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObjectManager.h"
#include "Transform.h"
#include "MeleeFollowMove.h"
#include "MeleeMove.h"
#include "Animater.h"
#include "Timer.h"
#include "Attack.h"

REGISTER_COMPONENT(TMF::Sword, "Sword");

namespace TMF
{
	void Sword::OnInitialize()
	{
		m_weaponType = WeaponType::MELEE;
		OwnerWeponCheck();
	}
	void Sword::OnFinalize()
	{
	}
	void Sword::OnUpdate()
	{
		auto deltaTime = Timer::Instance().deltaTime.count();
		if (m_isMelee == true)
		{
			m_meleeTimer += deltaTime;
			if (m_meleeTimer > m_endTime + m_animationStopTime)
			{
				m_isMeleeEnd = true;
				m_isMelee = false;
			}
			if (m_timer > m_endTime + m_animationStopTime)
			{
				m_isMeleeEnd = true;
			}

		}
		else
		{
			if (m_meleeTimer != 0.0f)
			{
				m_timer += deltaTime;
				if (m_timer > m_coolTime)
				{
					m_timer = 0.0f;
					m_meleeTimer = 0.0f;
				}
			}
		}
		if (m_lateTimer > 0.0f)
		{
			m_lateTimer -= deltaTime;
		}
		else
		{
			if (m_lateTimer <= 0.0f)
			{
				if (m_changeTime < m_initChangeTime)
				{
					m_changeTime += deltaTime;
				}
			}
		}
	}
	void Sword::OnLateUpdate()
	{
	}
	void Sword::OnDraw()
	{
	}
	void Sword::OnDrawImGui()
	{
		char animBuf[256] = "";
		strcpy_s(animBuf, sizeof(animBuf), m_meleeAnimation.c_str());
		auto meleeAnimPath = StringHelper::CreateLabel("AnimPath", m_uuID);
		if (ImGui::InputText(meleeAnimPath.c_str(), animBuf, 256))
		{
			m_meleeAnimation = animBuf;
		}

		auto endTimeLabel = StringHelper::CreateLabel("EndTime", m_uuID);
		if (ImGui::DragFloat(endTimeLabel.c_str(), &m_endTime))
		{

		}

		auto cancelTimeLabel = StringHelper::CreateLabel("CancelTime", m_uuID);
		if (ImGui::DragFloat(cancelTimeLabel.c_str(), &m_cancelTime))
		{

		}

		auto coolTimeLabel = StringHelper::CreateLabel("CoolTime", m_uuID);
		if (ImGui::DragFloat(coolTimeLabel.c_str(), &m_coolTime))
		{

		}

		auto animationStopTimeLabel = StringHelper::CreateLabel("AnimationStopTime", m_uuID);
		if (ImGui::DragFloat(animationStopTimeLabel.c_str(), &m_animationStopTime, 0.1f))
		{

		}

		auto animationSpeedLabel = StringHelper::CreateLabel("AnimationSpeed", m_uuID);
		if (ImGui::DragFloat(animationSpeedLabel.c_str(), &m_animationSpeed))
		{

		}

		auto playLabel = StringHelper::CreateLabel("Play", m_uuID);
		if (ImGui::Button(playLabel.c_str()))
		{
			//OnAttack();
			Cancel();
		}

	}
	std::shared_ptr<Component> Sword::OnClone() const
	{
		auto pClone = std::make_shared<Sword>();
		pClone->m_meleeAnimation = this->m_meleeAnimation;
		pClone->m_endTime = this->m_endTime;
		pClone->m_animationSpeed = this->m_animationSpeed;
		return move(pClone);
	}
	float Sword::GetMeleeTime()
	{
		return m_meleeTimer;
	}
	float Sword::GetMeleeStopTime()
	{
		return m_animationStopTime;
	}
	bool Sword::OnAttack()
	{
		if (m_meleeTimer != 0.0f || m_changeTime < m_initChangeTime)
		{
			return false;
		}

		if (auto pLockOwner = m_pOwner.lock())
		{
			auto parentPosition = DirectX::SimpleMath::Vector3::Zero;
			auto parentRotation = DirectX::SimpleMath::Quaternion::Identity;
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				auto pParentTransform = pLockTransform->GetParent();
				if (auto pLockParentTranform = pParentTransform.lock())
				{
					// 親の座標を取得
					parentPosition = pLockParentTranform->GetPosition();
					parentRotation = pLockParentTranform->GetRotation();
					m_pParent = pLockParentTranform->GetOwner();
				}
			}
			m_pMeleeMove = pLockOwner->GetComponent<MeleeMove>();
			if (auto pLockMeleemove = m_pMeleeMove.lock())
			{
				// 追従を解除
				auto pMeleeFollowMove = pLockOwner->GetComponent<MeleeFollowMove>();
				if (auto pLockMeleeFollowMove = pMeleeFollowMove.lock())
				{
					pLockMeleeFollowMove->SetIsEnable(false);
				}
				pLockMeleemove->Play(MeleeMove::DEFAULT, parentPosition, parentRotation);
				m_isMelee = true;
				m_meleeTimer = 0;
			}
			// 親子関係を外す　親を
			if (m_isMelee == true)
			{
				auto pTransform = pLockOwner->GetComponent<Transform>();
				if (auto pLockTransform = pTransform.lock())
				{
					pLockTransform->SetParent(std::weak_ptr<Transform>());
				}

				if (auto pLockParentOwner = m_pParent.lock())
				{
					auto pAnimater = pLockParentOwner->GetComponent<Animater>();
					if (auto pLockAnimater = pAnimater.lock())
					{
						// アニメーションのパスの変更
						pLockAnimater->SetFileName(m_meleeAnimation, m_endTime, m_animationSpeed, m_animationStopTime);

						// 変更　攻撃の全体フレーム時間とアニメーションを止める時間を設定するように
					}
					m_isMeleeEnd = false;
					m_isCancel = false;
					return true;
				}
			}
		}
		return false;
	}
	void Sword::OnSelect()
	{
		m_changeTime = 0.0f;
	}
	void Sword::OnCancel()
	{
		if (auto pLockParentOwner = m_pParent.lock())
		{
			auto pParentAnimater = pLockParentOwner->GetComponent<Animater>();
			if (auto pLockParentAnimater = pParentAnimater.lock())
			{
				if (auto pLockMeleeMove = m_pMeleeMove.lock())
				{
					pLockMeleeMove->Cancel();
				}

				pLockParentAnimater->LoadDefaltAnimation();
				m_isMelee = false;
				m_lateTimer = 0.0f;
				//m_meleeTimer = 0.0f;
				m_timer = 0.0f;
				m_isCancel = true;
			}
		}
	}
}