#include "EnemyController.h"

#include "State/StateMachine.h"

namespace TMF
{
	void EnemyController::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pStateMachine = std::make_shared<StateMachine>(pLockOwner);
		}
	}
	void EnemyController::OnFinalize()
	{

	}
	void EnemyController::OnUpdate()
	{
		if (m_pStateMachine)
		{
			m_pStateMachine->Update();
		}
	}
	void EnemyController::OnLateUpdate()
	{

	}
	void EnemyController::OnDraw()
	{

	}
	void EnemyController::OnDrawImGui()
	{

	}
}
