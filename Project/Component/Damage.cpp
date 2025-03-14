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
#include "Effect.h"

REGISTER_COMPONENT(TMF::Damage, "Damage");

namespace TMF
{
	void Damage::OnInitialize()
	{
		OwnerCheck();
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
		const char* types[] = { "Stagger", "Invert", "None" };
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

	void Damage::OnTriggerEnter(GameObject* pGameObject)
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			if (pLockOwner->GetActive() == false)
			{
				return;
			}

			auto pMeleeMove = pLockOwner->GetComponent<MeleeMove>();
			if (auto pLockMeleeMove = pMeleeMove.lock())
			{
				if (pLockMeleeMove->GetIsPlay() == false)
				{
					return;
				}
			}

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
					auto pPlayerStatus = pGameObject->GetComponent<PlayerStatus>();
					if (auto pLockPlayerStatus = pPlayerStatus.lock())
					{
						if (pLockPlayerStatus->GetIsInvincible() == true)
						{
							return;
						}
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
					// Audioを取得
					auto pAudio = pLockOwner->GetComponent<Audio>();
					if (auto pLockAudio = pAudio.lock())
					{
						// 音を鳴らす
						pLockAudio->Play();
					}
					// Effectを取得
					auto pEffect = pLockOwner->GetComponent<Effect>();
					if (auto pLockEffect = pEffect.lock())
					{
						pLockEffect->Play();
					}

				}
			}

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
	std::shared_ptr<Component> Damage::OnClone() const
	{
		auto pClone = std::make_shared<Damage>();
		pClone->m_damage = this->m_damage;
		pClone->m_reactionType = this->m_reactionType;
		return move(pClone);
	}
	void Damage::OwnerCheck()
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
}