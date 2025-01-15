#include "Melee.h"

#include <Imgui/imgui.h>

#include "Animater.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "Transform.h"
#include "PrimitiveMesh.h"
#include "Animater.h"
#include "MeleeMove.h"
#include "MeleeFollowMove.h"
#include "Timer.h"

REGISTER_COMPONENT(TMF::Melee, "Melee");

namespace TMF
{
	void Melee::OnInitialize()
	{
		m_changeTime = m_initChangeTime;
	}
	void Melee::OnFinalize()
	{
	}
	void Melee::OnUpdate()
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
	void Melee::OnLateUpdate()
	{
	}
	void Melee::OnDraw()
	{
	}
	void Melee::OnDrawImGui()
	{
		char animBuf[256] = "";
		strcpy_s(animBuf, sizeof(animBuf), m_meleeAnimation.c_str());
		auto meleeAnimPath = StringHelper::CreateLabel("AnimPath", m_uuID);
		if (ImGui::InputText(meleeAnimPath.c_str(), animBuf, 256))
		{
			m_meleeAnimation = animBuf;
		}

		char meleeObjectNameBuf[256] = "";
		strcpy_s(meleeObjectNameBuf, sizeof(meleeObjectNameBuf), m_meleeObjectName.c_str());
		auto meleeObjectNameLabel = StringHelper::CreateLabel("MeleeObjectName", m_uuID);
		if (ImGui::InputText(meleeObjectNameLabel.c_str(), meleeObjectNameBuf, 256))
		{
			m_meleeObjectName = meleeObjectNameBuf;
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
			Play();
		}
	}
	void Melee::Play()
	{
		if (m_meleeObjectName == "")
		{
			return;
		}
		if (m_isMelee == true || m_changeTime < m_initChangeTime)
		{
			return;
		}
		auto nowPosition = DirectX::SimpleMath::Vector3::Zero;
		auto nowRotation = DirectX::SimpleMath::Quaternion::Identity;

		if (auto pLockOwner = m_pOwner.lock())
		{

			// 座標の取得
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				nowPosition = pLockTransform->GetPosition();
				nowRotation = pLockTransform->GetRotation();
			}
			auto pChildren = pLockOwner->GetChildren();
			for (auto& pChild : pChildren)
			{
				if (auto pLockChild = pChild.lock())
				{
					if (pLockChild->GetName() == m_meleeObjectName)
					{
						auto pMeleeMove = pLockChild->GetComponent<MeleeMove>();
						if (auto pLockMeleemove = pMeleeMove.lock())
						{
							// 追従を解除
							auto pMeleeFollowMove = pLockChild->GetComponent<MeleeFollowMove>();
							if (auto pLockMeleeFollowMove = pMeleeFollowMove.lock())
							{
								pLockMeleeFollowMove->SetIsEnable(false);
							}
							pLockMeleemove->Play(MeleeMove::DEFAULT, nowPosition, nowRotation);
							m_isMelee = true;
						}
						if (m_isMelee == true)
						{
							auto pChildTransform = pLockChild->GetComponent<Transform>();
							if (auto pLockChildTransform = pChildTransform.lock())
							{
								pLockChildTransform->SetParent(std::weak_ptr<Transform>());
							}
						}
					}
				}
			}
			if (m_isMelee == true)
			{
				auto pAnimater = pLockOwner->GetComponent<Animater>();
				if (auto pLockAnimater = pAnimater.lock())
				{
					// アニメーションのパスの変更
					pLockAnimater->SetFileName(m_meleeAnimation, m_endTime);
				}
				m_isMeleeEnd = false;
			}
		}
	}
	void Melee::Select()
	{
		m_changeTime = 0.0f;;
	}
}