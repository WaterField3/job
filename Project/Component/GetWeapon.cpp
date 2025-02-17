#include "GetWeapon.h"

#include "ComponentRegister.h"
#include "GameObject/GameObject.h"
#include "Utility/Log.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "GhostObject.h"
#include "Attack.h"
#include "MeleeMove.h"
#include "Damage.h"

REGISTER_COMPONENT(TMF::GetWeapon, "GetWeapon");

namespace TMF
{
	void GetWeapon::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				m_pTransform = pLockTransform;
			}
		}
	}
	void GetWeapon::OnFinalize()
	{
	}
	void GetWeapon::OnUpdate()
	{
	}
	void GetWeapon::OnLateUpdate()
	{
	}
	void GetWeapon::OnDraw()
	{
	}
	void GetWeapon::OnDrawImGui()
	{
	}
	void GetWeapon::OnCollisionEnter(GameObject* pGameObject)
	{
		if (pGameObject->GetTag() == GameObject::Tag::Item)
		{
			auto pHitGameObjectTransform = pGameObject->GetComponent<Transform>();
			if (auto pLockHitGameObjectTransform = pHitGameObjectTransform.lock())
			{
				if (auto pLockTransform = m_pTransform.lock())
				{
					pLockHitGameObjectTransform->SetParent(pLockTransform);

					pGameObject->SetTag(GameObject::Tag::Default);
					// e‚Ìî•ñ‚ð‘ã“ü‚·‚é
					auto pMeleeMove = pGameObject->GetComponent<MeleeMove>();
					if (auto pLockMeleeMove = pMeleeMove.lock())
					{
						pLockMeleeMove->CheckParent();
					}
				}
			}

			auto pDamage = pGameObject->GetComponent<Damage>();
			if (auto pLockDamage = pDamage.lock())
			{
				pLockDamage->OwnerCheck();
			}

			pGameObject->RemoveComponent<Rigidbody>();
			pGameObject->AddComponent<GhostObject>();
			if (auto pLockOwner = m_pOwner.lock())
			{
				auto pAttack = pLockOwner->GetComponent<Attack>();
				if (auto pLockAttack = pAttack.lock())
				{
					pLockAttack->WeponsUpdate();
				}
			}
		}
	}
	std::shared_ptr<Component> GetWeapon::OnClone() const
	{
		auto pClone = std::make_shared<GetWeapon>();
		return move(pClone);
	}
}