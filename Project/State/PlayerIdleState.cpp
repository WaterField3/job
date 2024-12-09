#include "PlayerIdleState.h"

#include "StateRegister.h"
#include "Utility/Log.h"
#include "PlayerMove.h"
#include "EventSystem/EventSystem.h"
#include "Component/Transform.h"
#include "Component/Rigidbody.h" 
#include "Input.h"
#include "StateMachine.h"

REGISTER_STATE(TMF::PlayerIdleState, "PlayerIdleState");

namespace TMF
{
	PlayerIdleState::PlayerIdleState()
	{

	}

	PlayerIdleState::~PlayerIdleState()
	{

	}
	void PlayerIdleState::OnInitialize()
	{
		m_pEventSystem = std::make_unique<EventSystem>();
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransfrom = pTransform.lock())
			{
				auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
				if (auto pLockRigidbody = pRigidbody.lock())
				{
					m_pPlayerMove = std::make_unique<PlayerMove>(pLockTransfrom, pLockRigidbody, 1.0f);
					m_pEventSystem->AddHandler('w', [this]() {m_pPlayerMove->MoveForward(); });
					m_pEventSystem->AddHandler('a', [this]() {m_pPlayerMove->MoveLeft(); });
					m_pEventSystem->AddHandler('s', [this]() {m_pPlayerMove->MoveBack(); });
					m_pEventSystem->AddHandler('d', [this]() {m_pPlayerMove->MoveRight(); });
				}
			}

		}
	}
	void PlayerIdleState::OnEnter()
	{
		//std::string enter = { "PlayerIdleStateEnter\n" };
		//Log::Info("%s",enter.c_str());
	}
	void PlayerIdleState::OnUpdate()
	{
		auto keyState = Input::Instance().GetKeyState();
		if (keyState.W == true)
		{
			m_pEventSystem->TriggerEvent('w');
		}
		if (keyState.A == true)
		{
			m_pEventSystem->TriggerEvent('a');
		}
		if (keyState.S == true)
		{
			m_pEventSystem->TriggerEvent('s');
		}
		if (keyState.D == true)
		{
			m_pEventSystem->TriggerEvent('d');
		}
		if (keyState.Space == true)
		{
			if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
			{
				pLockAdministratorStateMachine->ChangeState("PlayerJumpState");
			}
		}
	}
	void PlayerIdleState::OnExit()
	{

	}
}
