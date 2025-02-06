#include "Sword.h"

#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Transform.h"
#include "MeleeFollowMove.h"
#include "MeleeMove.h"
#include "Animater.h"
#include "Timer.h"

REGISTER_COMPONENT(TMF::Sword, "Sword");

namespace TMF
{
	void Sword::OnInitialize()
	{
	}
	void Sword::OnFinalize()
	{
	}
	void Sword::OnUpdate()
	{
		if (m_isMelee == true)
		{
			m_timer += Timer::Instance().deltaTime.count();
			if (m_timer > m_endTime)
			{
				m_isMeleeEnd = true;
			}
			if (m_timer > m_coolTime)
			{
				m_isMelee = false;
				m_timer = 0.0f;
			}
		}
		if (m_changeTime < m_initChangeTime)
		{
			m_changeTime += Timer::Instance().deltaTime.count();
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

		auto coolTimeLabel = StringHelper::CreateLabel("CoolTime", m_uuID);
		if (ImGui::DragFloat(coolTimeLabel.c_str(), &m_coolTime))
		{

		}

		auto playLabel = StringHelper::CreateLabel("Play", m_uuID);
		if (ImGui::Button(playLabel.c_str()))
		{
			OnAttack();
		}
	}
	std::shared_ptr<Component> Sword::OnClone() const
	{
		auto pClone = std::make_shared<Sword>();
		pClone->m_meleeAnimation = this->m_meleeAnimation;
		pClone->m_endTime = this->m_endTime;
		return move(pClone);
	}
	void Sword::OnAttack()
	{
		if (m_isMelee == true || m_changeTime < m_initChangeTime)
		{
			return;
		}

		if (auto pLockOwner = m_pOwner.lock())
		{
			auto parentPosition = DirectX::SimpleMath::Vector3::Zero;
			auto parentRotation = DirectX::SimpleMath::Quaternion::Identity;
			auto pParentOwner = std::weak_ptr<GameObject>();
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				auto pParentTransform = pLockTransform->GetParent();
				if (auto pLockParentTranform = pParentTransform.lock())
				{
					// 親の座標を取得
					parentPosition = pLockParentTranform->GetPosition();
					parentRotation = pLockParentTranform->GetRotation();
					pParentOwner = pLockParentTranform->GetOwner();
				}
			}
			auto pMeleeMove = pLockOwner->GetComponent<MeleeMove>();
			if (auto pLockMeleemove = pMeleeMove.lock())
			{
				// 追従を解除
				auto pMeleeFollowMove = pLockOwner->GetComponent<MeleeFollowMove>();
				if (auto pLockMeleeFollowMove = pMeleeFollowMove.lock())
				{
					pLockMeleeFollowMove->SetIsEnable(false);
				}
				pLockMeleemove->Play(MeleeMove::DEFAULT, parentPosition, parentRotation);
				m_isMelee = true;
			}
			// 親子関係を外す　親を
			if (m_isMelee == true)
			{
				auto pTransform = pLockOwner->GetComponent<Transform>();
				if (auto pLockTransform = pTransform.lock())
				{
					pLockTransform->SetParent(std::weak_ptr<Transform>());
				}

				if (auto pLockParentOwner = pParentOwner.lock())
				{
					auto pAnimater = pLockParentOwner->GetComponent<Animater>();
					if (auto pLockAnimater = pAnimater.lock())
					{
						// アニメーションのパスの変更
						pLockAnimater->SetFileName(m_meleeAnimation, m_endTime);
					}
					m_isMeleeEnd = false;
				}
			}
		}
	}
	void Sword::OnSelect()
	{
		m_changeTime = 0.0f;
	}
}