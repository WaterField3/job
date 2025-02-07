#include "GetWepon.h"

#include "ComponentRegister.h"
#include "GameObject/GameObject.h"
#include "Utility/Log.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "GhostObject.h"
#include "Attack.h"
#include "MeleeMove.h"

REGISTER_COMPONENT(TMF::GetWepon, "GetWepon");

namespace TMF
{
	void GetWepon::OnInitialize()
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
	void GetWepon::OnFinalize()
	{
	}
	void GetWepon::OnUpdate()
	{
	}
	void GetWepon::OnLateUpdate()
	{
	}
	void GetWepon::OnDraw()
	{
	}
	void GetWepon::OnDrawImGui()
	{
	}
	void GetWepon::OnCollisionEnter(GameObject* pGameObject)
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
	std::shared_ptr<Component> GetWepon::OnClone() const
	{
		auto pClone = std::make_shared<GetWepon>();
		return move(pClone);
	}
}