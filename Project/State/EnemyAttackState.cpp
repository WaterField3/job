#include "EnemyAttackState.h"

#include "GameObject/GameObjectManager.h"
#include "Component/Transform.h"
#include "Component/EnemyAttack.h"
#include "Component/EnemyMove.h"
#include "StateMachine.h"
#include "StateRegister.h"
#include "Timer.h"

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
			auto pEnemyAttack = pLockOwner->GetComponent<EnemyAttack>();
			if (auto pLockEnemyAttack = pEnemyAttack.lock())
			{
				m_pEnemyAttack = pLockEnemyAttack;
				if (pLockEnemyAttack->Play() == true)
				{
					m_isPlay = true;
				}
			}
			m_pEnemyMove = pLockOwner->GetComponent<EnemyMove>();

		}
	}
	void EnemyAttackState::OnEnter()
	{

	}
	void EnemyAttackState::OnUpdate()
	{
		if (m_isPlay == true)
		{
			auto deltaTime = Timer::Instance().deltaTime.count();
			m_endTime -= deltaTime;
			if (m_endTime <= 0.0f)
			{
				if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
				{
					pLockAdministratorStateMachine->ChangeState("EnemyIdleState");
					return;
				}
			}
		}
		else
		{
			if (auto pLockEnemyAttack = m_pEnemyAttack.lock())
			{
				if (pLockEnemyAttack->Play() == true)
				{
					m_isPlay = true;
				}
			}
		}
	}
	void EnemyAttackState::OnLateUpdate()
	{
		if (m_isPlay == false)
		{
			if (auto pLockEnemyMove = m_pEnemyMove.lock())
			{
				pLockEnemyMove->Move();
			}
		}
	}
	void EnemyAttackState::OnExit()
	{

	}
}