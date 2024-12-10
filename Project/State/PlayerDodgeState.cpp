#include "PlayerDodgeState.h"

#include "StateRegister.h"
#include "GameObject/GameObject.h"
#include "Component/Dodge.h"
#include "MoveInfo.h"
#include "EventSystem/EventSystem.h"
#include "Input.h"
#include "State/StateMachine.h"

REGISTER_STATE(TMF::PlayerDodgeState, "PlayerDodgeState");

namespace TMF
{
	PlayerDodgeState::PlayerDodgeState()
	{

	}
	PlayerDodgeState::~PlayerDodgeState()
	{

	}
	void PlayerDodgeState::OnInitialize()
	{

	}
	void PlayerDodgeState::OnEnter()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pDodge = pLockOwner->GetComponent<Dodge>();
			auto moveDirection = MoveDirection::NEUTRAL;
			auto keyState = Input::Instance().GetKeyState();
			if (keyState.W == true)
			{
				moveDirection = MoveDirection::FOWARD;
			}
			else if (keyState.A == true)
			{
				moveDirection = MoveDirection::LEFT;
			}
			else if (keyState.S == true)
			{
				moveDirection = MoveDirection::BACK;
			}
			else if (keyState.D == true)
			{
				moveDirection = MoveDirection::RIGHT;
			}
			if (auto pLockDodge = m_pDodge.lock())
			{
				pLockDodge->DodgeStart(moveDirection);
			}
		}
	}
	void PlayerDodgeState::OnUpdate()
	{
		if (auto pLockDodge = m_pDodge.lock())
		{
			if (pLockDodge->GetIsDodgeEnd() == true)
			{
				if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
				{
					pLockAdministratorStateMachine->ChangeState("PlayerIdleState");
				}
			}
		}
	}
	void PlayerDodgeState::OnExit()
	{

	}
}
