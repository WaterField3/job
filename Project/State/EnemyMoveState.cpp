#include "EnemyMoveState.h"

#include "StateRegister.h"
#include "Component/EnemyMove.h"
#include "GameObject/GameObject.h"
#include "State/StateMachine.h"

REGISTER_STATE(TMF::EnemyMoveState, "EnemyMoveState");

namespace TMF
{
	void EnemyMoveState::OnInitialize()
	{
		
	}
	void EnemyMoveState::OnEnter()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pEnemyMove = pLockOwner->GetComponent<EnemyMove>();
		}
	}
	void EnemyMoveState::OnUpdate()
	{
		if (auto pLockEnemyMove = m_pEnemyMove.lock())
		{
			if (5.0f > pLockEnemyMove->GetPlayerDistance())
			{
				if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
				{
					//pLockAdministratorStateMachine->ChangeState("EnemyIdleState");
					pLockAdministratorStateMachine->ChangeState("EnemyAttackState");
				}
			}
		}
	}
	void EnemyMoveState::OnLateUpdate()
	{
		if (auto pLockEnemyMove = m_pEnemyMove.lock())
		{
			pLockEnemyMove->Move();
		}
	}
	void EnemyMoveState::OnExit()
	{

	}
}
