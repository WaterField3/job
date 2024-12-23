#include "EnemyAttackState.h"

#include "GameObject/GameObjectManager.h"
#include "Component/Transform.h"
#include "Component/EnemyAttack.h"
#include "StateMachine.h"
#include "StateRegister.h"

REGISTER_STATE(TMF::EnemyAttackState, "EnemyAttackState");


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
		if (auto pLockTransform = m_pTransform.lock())
		{
			if (auto pLockPlayerTransform = m_pPlayerTransform.lock())
			{
				auto pos = pLockTransform->GetPosition();
				auto playerPos = pLockPlayerTransform->GetPosition();

				auto distance = DirectX::SimpleMath::Vector3::Distance(pos, playerPos);

				if (auto pLockEnemyAttack = m_pEnemyAttack.lock())
				{
					if (distance < m_shotDistance)
					{
						pLockEnemyAttack->SelectMelee();
						m_isMeleePlay = true;
					}
					else
					{
						pLockEnemyAttack->SelectShot();
					}
					pLockEnemyAttack->Play();
				}
			}
		}
	}
	void EnemyAttackState::OnUpdate()
	{
		if (m_isMeleePlay == true)
		{
			if (auto pLockEnemyAttack = m_pEnemyAttack.lock())
			{
				if (pLockEnemyAttack->GetIsMeleeEnd() == true)
				{
					if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
					{
						pLockAdministratorStateMachine->ChangeState("EnemyMoveState");
					}
				}
			}
		}
		else
		{
			if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
			{
				pLockAdministratorStateMachine->ChangeState("EnemyMoveState");
			}
		}

	}
	void EnemyAttackState::OnLateUpdate()
	{

	}
	void EnemyAttackState::OnExit()
	{

	}
}