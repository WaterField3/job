#include "EnemyController.h"

#include "State/StateMachine.h"
#include "ComponentRegister.h"

REGISTER_COMPONENT(TMF::EnemyController, "EnemyController");

namespace TMF
{
	void EnemyController::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pStateMachine = std::make_shared<StateMachine>(pLockOwner);
			m_pStateMachine->ChangeState("EnemyMoveState");
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
		if (m_pStateMachine)
		{
			m_pStateMachine->LateUpdate();
		}
	}
	void EnemyController::OnDraw()
	{

	}
	void EnemyController::OnDrawImGui()
	{

	}
	std::shared_ptr<Component> EnemyController::OnClone() const
	{
		auto pClone = std::make_shared<EnemyController>();
		return move(pClone);
	}
}
