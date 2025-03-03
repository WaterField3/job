#include "EnemyIdleState.h"

#include "StateRegister.h"
#include "State/StateMachine.h"
#include "Component/Transform.h"
#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "Component/EnemyMove.h"
#include "Component/EnemyAttack.h"

REGISTER_STATE(TMF::EnemyIdleState, "EnemyIdleState");

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
						m_pPlayerTransform = pLockPlayerTagObject->GetComponent<Transform>();
					}
				}
			}
			m_pTransform = pLockOwner->GetComponent<Transform>();
			m_pEnemyMove = pLockOwner->GetComponent<EnemyMove>();
			m_pEnemyAttack = pLockOwner->GetComponent<EnemyAttack>();
		}
	}
	void EnemyIdleState::OnEnter()
	{

	}
	void EnemyIdleState::OnUpdate()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			if (auto pLockPlayerTransform = m_pPlayerTransform.lock())
			{
				auto pos = pLockTransform->GetPosition();
				auto playerPos = pLockPlayerTransform->GetPosition();

				auto distance = DirectX::SimpleMath::Vector3::Distance(pos, playerPos);

				if (auto pLockEnemyAttack = m_pEnemyAttack.lock())
				{
					auto shotDistance = pLockEnemyAttack->GetShotDinstance();
					if (distance < shotDistance)
					{
						pLockEnemyAttack->SelectMelee();
						if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
						{
							pLockAdministratorStateMachine->ChangeState("EnemyAttackState");
						}
					}
					else
					{
						pLockEnemyAttack->SelectShot();
					}
				}
			}
		}
	}
	void EnemyIdleState::OnLateUpdate()
	{
		if (auto pLockEnemyMove = m_pEnemyMove.lock())
		{
			pLockEnemyMove->Move();
		}
	}
	void EnemyIdleState::OnExit()
	{

	}
}
