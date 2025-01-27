#include "PlayerAttackState.h"

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "StateMachine.h"
#include "StateRegister.h"
#include "Component/Attack.h"
#include "Component/Rigidbody.h"
#include "Timer.h"

REGISTER_STATE(TMF::PlayerAttackState, "PlayerAttackState");

namespace TMF
{
	void PlayerAttackState::OnInitialize()
	{

	}
	void PlayerAttackState::OnEnter()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			if (auto pLockRigidbody = pRigidbody.lock())
			{
				pLockRigidbody->SetLinearVelocity(DirectX::SimpleMath::Vector3::Zero);
			}
			auto pAttack = pLockOwner->GetComponent<Attack>();
			if (auto pLockAttack = pAttack.lock())
			{
				m_endTime = pLockAttack->Play();
			}
		}
	}
	void PlayerAttackState::OnUpdate()
	{
		m_endTime -= Timer::Instance().deltaTime.count();
		if (m_endTime <= 0)
		{
			if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
			{
				pLockAdministratorStateMachine->ChangeState("PlayerIdleState");
			}
		}
	}
	void PlayerAttackState::OnLateUpdate()
	{
	}
	void PlayerAttackState::OnExit()
	{
	}
}