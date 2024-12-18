#include "EnemyAttackState.h"

#include "GameObject/GameObjectManager.h"
#include "Component/Transform.h"
#include "Component/EnemyAttack.h"

namespace TMF
{
	void EnemyAttackState::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pTransform = pLockOwner->GetComponent<Transform>();
			auto playerTagObjects = GameObjectManager::Instance().GetGameObjects(static_cast<int>(GameObject::Tag::Player));
			auto size = playerTagObjects.size();
			for (auto i = 0; i < size; i++)
			{
				if (auto pLockPlayerTagObject = playerTagObjects[i].lock())
				{
					if (pLockOwner != pLockPlayerTagObject)
					{
						m_pPlayerTransform = pLockPlayerTagObject->GetComponent<Transform>();
					}
				}
			}
			m_pEnemyAttack = pLockOwner->GetComponent<EnemyAttack>();
		}
	}
	void EnemyAttackState::OnEnter()
	{

	}
	void EnemyAttackState::OnUpdate()
	{
		
	}
	void EnemyAttackState::OnLateUpdate()
	{

	}
	void EnemyAttackState::OnExit()
	{

	}
}