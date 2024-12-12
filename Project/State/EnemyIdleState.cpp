#include "EnemyIdleState.h"

#include "Component/Transform.h"
#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"

namespace TMF
{
	void EnemyIdleState::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto playerTagObjects = GameObjectManager::Instance().GetGameObjects(static_cast<int>(GameObject::Tag::Player));
			auto size = playerTagObjects.size();
			for (auto i = 0; i < size; i++)
			{
				if (auto pLockPlayerTagObject = playerTagObjects[i].lock())
				{
					if (pLockOwner != pLockPlayerTagObject)
					{
						m_pPlayerTransfrom = pLockPlayerTagObject->GetComponent<Transform>();
					}
				}
			}
			
		}
	}
	void EnemyIdleState::OnEnter()
	{

	}
	void EnemyIdleState::OnUpdate()
	{

	}
	void EnemyIdleState::OnExit()
	{

	}
}
