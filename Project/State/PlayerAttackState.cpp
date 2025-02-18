#include "PlayerAttackState.h"

#include "GameObject/GameObjectManager.h"
#include "GameObject/GameObject.h"
#include "StateMachine.h"
#include "StateRegister.h"
#include "Component/Attack.h"
#include "Component/Rigidbody.h"
#include "Component/Thruster.h"
#include "Timer.h"
#include "Input.h"

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
				auto weaponActionTiming = pLockAttack->Play();
				m_endTime = weaponActionTiming.attackEndTiming;
				m_cancelTime = weaponActionTiming.attackCancelTiming;
			}
			auto pThruster = pLockOwner->GetComponent<Thruster>();
			if (auto pLockThruster = pThruster.lock())
			{
				m_pThruster = pLockThruster;
			}
		}
	}
	void PlayerAttackState::OnUpdate()
	{
		auto deltaTime = Timer::Instance().deltaTime.count();
		m_endTime -= deltaTime;
		m_cancelTime -= deltaTime;
		if (m_endTime <= 0.0f)
		{
			if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
			{
				pLockAdministratorStateMachine->ChangeState("PlayerIdleState");
				return;
			}
		}
		if (auto pLockThruster = m_pThruster.lock())
		{
			if (m_cancelTime <= 0.0f)
			{
				auto keyState = Input::Instance().GetKeyState();
				if (keyState.V == true && pLockThruster->GetIsOverHeat() == false)
				{
					if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
					{
						pLockAdministratorStateMachine->ChangeState("PlayerIdleState");
						return;
					}
				}
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