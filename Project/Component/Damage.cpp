#include "Damage.h"

#include <Imgui/imgui.h>

#include "GameObject/GameObject.h"
#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Audio.h"
#include "BulletStraightMove.h"
#include "MeleeMove.h"
#include "Transform.h"
#include "PlayerStatus.h"

REGISTER_COMPONENT(TMF::Damage, "Damage");

namespace TMF
{
	void Damage::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = m_pTransform.lock())
			{
				m_pParent = pLockTransform->GetParent();
			}
		}
	}
	void Damage::OnFinalize()
	{
	}
	void Damage::OnUpdate()
	{
	}
	void Damage::OnLateUpdate()
	{
	}
	void Damage::OnDraw()
	{
	}
	void Damage::OnDrawImGui()
	{
		auto damageLabel = StringHelper::CreateLabel("DamageValue", m_uuID);
		if (ImGui::DragFloat(damageLabel.c_str(), &m_damage))
		{

		}
		const char* types[] = { "Stagger","Invert","None" };
		int selectIndex = (int)m_reactionType;
		auto shapeLabel = StringHelper::CreateLabel("ReactionType", m_uuID);
		if (ImGui::BeginCombo(shapeLabel.c_str(), types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_reactionType == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_reactionType = DamageReactionType(i);
					selectIndex = i;

				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
	}

	void Damage::OnTrigerEnter(GameObject* pGameObject)
	{

		if (auto pLockParent = m_pParent.lock())
		{
			if (auto pLockParentOwner = pLockParent->GetOwner().lock())
			{
				if (pGameObject == pLockParentOwner.get())
				{
					return;
				}
			}
		}

		// 当たったオブジェクトを確認　プレイヤーの時
		if (pGameObject->GetTag() == GameObject::Tag::Player)
		{
			if (auto pLockOwner = m_pOwner.lock())
			{
				auto pAudio = pLockOwner->GetComponent<Audio>();
				if (auto pLockAudio = pAudio.lock())
				{
					pLockAudio->Play();
				}
				auto pPlayerStatus = pGameObject->GetComponent<PlayerStatus>();
				if (auto pLockPlayerStatus = pPlayerStatus.lock())
				{
					switch (m_reactionType)
					{
					case TMF::Damage::STAGGER:
						pLockPlayerStatus->Stagger();
						break;
					case TMF::Damage::INVERT:
						pLockPlayerStatus->Invert();
						break;
					case TMF::Damage::NONE:
						break;
					default:
						break;
					}
					pLockPlayerStatus->Damage(m_damage);
				}
			}
		}
		if (auto pLockOwner = m_pOwner.lock())
		{
			// 弾での攻撃時は当たればすぐに消える
			auto pBulletMove = pLockOwner->GetComponent<BulletStraightMove>();
			if (auto pLockBulletMove = pBulletMove.lock())
			{
				pLockOwner->SetActive(false);
				if (auto pLockTransform = m_pTransform.lock())
				{
					pLockTransform->SetParent(m_pParent);
				}
			}
		}
	}
	void Damage::OnCollisionEnter(GameObject* pGameObject)
	{

		if (auto pLockParent = m_pParent.lock())
		{
			if (auto pLockParentOwner = pLockParent->GetOwner().lock())
			{
				if (pGameObject == pLockParentOwner.get())
				{
					return;
				}
			}
		}

		if (pGameObject->GetTag() == GameObject::Tag::Player)
		{
			if (auto pLockOwner = m_pOwner.lock())
			{
				auto pAudio = pLockOwner->GetComponent<Audio>();
				if (auto pLockAudio = pAudio.lock())
				{
					pLockAudio->Play();
				}
			}
		}
	}
}