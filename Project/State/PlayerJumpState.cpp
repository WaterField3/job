#include "PlayerJumpState.h"

#include "EventSystem/EventSystem.h"
#include "State/StateRegister.h"
#include "Utility/Log.h"
#include "Component/Jump.h"
#include "GameObject/GameObject.h"
#include "PlayerJump.h"
#include "Component/Transform.h"
#include "Component/Rigidbody.h"
#include "Component/Thruster.h"
#include "Input.h"
#include "State/StateMachine.h"
#include "PhysicsManager.h"

REGISTER_STATE(TMF::PlayerJumpState, "PlayerJumpState");

namespace TMF
{
	void PlayerJumpState::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pJump = pLockOwner->GetComponent<Jump>();
			auto isGetJump = false;
			if (auto pLockJump = pJump.lock())
			{
				m_pJump = pLockJump;
				isGetJump = true;
			}
			m_pTransform = pLockOwner->GetComponent<Transform>();
			bool isGetTransform = false;
			if (auto pLockTransform = m_pTransform.lock())
			{
				isGetTransform = true;
			}
			auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			auto isGetRigidbody = false;
			if (auto pLockRigidbody = pRigidbody.lock())
			{
				isGetRigidbody = true;
			}
			auto pThruster = pLockOwner->GetComponent<Thruster>();
			auto isGetThruster = false;
			if (auto pLockThruster = pThruster.lock())
			{
				if (pLockThruster->GetIsOverHeat() == true)
				{
					if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
					{
						pLockAdministratorStateMachine->ChangeState("PlayerIdleState");
						return;
					}
				}
				isGetThruster = true;
			}
			if (isGetTransform == true && isGetRigidbody == true && isGetThruster == true && isGetJump)
			{
				m_pPlayerJump = std::make_unique<PlayerJump>(m_pTransform, pRigidbody, pThruster, pJump);
			}
		}
	}
	void PlayerJumpState::OnEnter()
	{
		//Log::Info("%s", "Enter PlayerJumpState");
	}
	void PlayerJumpState::OnUpdate()
	{
		if (m_pPlayerJump)
		{
			auto keyState = Input::Instance().GetKeyState();
			auto keyTracker = Input::Instance().GetKeyboardTracker();

			keyTracker->Update(keyState);
			if (auto pLockJump = m_pJump.lock())
			{
				if (keyState.Space == true)
				{
					pLockJump->Chage(m_moveDirection);
				}
				// チャージ解除　ジャンプ開始
				if (keyState.IsKeyUp(DirectX::Keyboard::Space))
				{
					pLockJump->ChageStop(m_moveDirection);
				}

				if (keyState.LeftControl)
				{
					pLockJump->Fall();
				}

				if (pLockJump->GetIsLanding() == true)
				{
					if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
					{
						pLockAdministratorStateMachine->ChangeState("PlayerIdleState");
					}
				}
			}
		}
	}
	void PlayerJumpState::OnLateUpdate()
	{

	}
	void PlayerJumpState::OnExit()
	{

	}
}
