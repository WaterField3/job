#include "PlayerIdleState.h"

#include "StateRegister.h"
#include "Utility/Log.h"
#include "PlayerMove.h"
#include "EventSystem/EventSystem.h"
#include "Component/Transform.h"
#include "Component/Rigidbody.h"
#include "Component/Thruster.h"
#include "Component/CharacterMoveController.h"
#include "Component/PlayerStatus.h"
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
			auto isGetTransform = false;
			auto isGetRigidbody = false;
			auto isGetThruster = false;
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				isGetTransform = true;
			}
			auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			if (auto pLockRigidbody = pRigidbody.lock())
			{
				isGetRigidbody = true;

			}
			auto pThruster = pLockOwner->GetComponent<Thruster>();
			if (auto pLockThruster = pThruster.lock())
			{
				isGetThruster = true;
			}
			auto moveSpeed = 1.0f;
			auto pCharacterMoveController = pLockOwner->GetComponent<CharacterMoveController>();
			if (auto pLockCharacterMoveController = pCharacterMoveController.lock())
			{
				moveSpeed = pLockCharacterMoveController->GetMoveSpeed();
			}
			auto pPlayerStatus = pLockOwner->GetComponent<PlayerStatus>();
			if (auto pLockPlayerStatus = pPlayerStatus.lock())
			{
				m_pPlayerStatus = pLockPlayerStatus;
			}
		
			if (isGetTransform == true && isGetRigidbody == true && isGetThruster == true)
			{
				m_pPlayerMove = std::make_unique<PlayerMove>(pTransform, pRigidbody, pThruster, moveSpeed);
				m_pEventSystem->AddHandler('w', [this]() {m_pPlayerMove->MoveForward(); });
				m_pEventSystem->AddHandler('a', [this]() {m_pPlayerMove->MoveLeft(); });
				m_pEventSystem->AddHandler('s', [this]() {m_pPlayerMove->MoveBack(); });
				m_pEventSystem->AddHandler('d', [this]() {m_pPlayerMove->MoveRight(); });
				m_pEventSystem->AddHandler('W', [this]() {m_pPlayerMove->FastMoveForward(); });
				m_pEventSystem->AddHandler('A', [this]() {m_pPlayerMove->FastMoveLeft(); });
				m_pEventSystem->AddHandler('S', [this]() {m_pPlayerMove->FastMoveBack(); });
				m_pEventSystem->AddHandler('D', [this]() {m_pPlayerMove->FastMoveRight(); });
				m_pEventSystem->AddHandler('N', [this]() {m_pPlayerMove->StopFastMove(); });

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

		auto mouseTracker = Input::Instance().GetMouseTracker();
		auto mouseState = Input::Instance().GetMouseState();
		mouseTracker->Update(mouseState);

		if (auto pLockPlayerStatus = m_pPlayerStatus.lock())
		{
			if (pLockPlayerStatus->GetIsMove() == false)
			{
				return;
			}
		}

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

		if (keyState.V == true)
		{
			if (keyState.A == true)
			{
				m_pEventSystem->TriggerEvent('A');
			}
			else if (keyState.S == true)
			{
				m_pEventSystem->TriggerEvent('S');
			}
			else if (keyState.D == true)
			{
				m_pEventSystem->TriggerEvent('D');
			}
			else
			{
				m_pEventSystem->TriggerEvent('W');
			}

		}
		else
		{
			m_pEventSystem->TriggerEvent('N');
		}
		if (keyState.Space == true)
		{
			if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
			{
				pLockAdministratorStateMachine->ChangeState("PlayerJumpState");
			}
		}
		if (mouseTracker->leftButton == DirectX::Mouse::ButtonStateTracker::ButtonState::PRESSED)
		{
			if (auto pLockAdministratorStateMachine = m_pAdministratorStateMachine.lock())
			{
				pLockAdministratorStateMachine->ChangeState("PlayerAttackState");
			}
		}
	}
	void PlayerIdleState::OnLateUpdate()
	{
	}
	void PlayerIdleState::OnExit()
	{

	}
}
