#include "PlayerJumpState.h"

#include "EventSystem/EventSystem.h"
#include "State/StateRegister.h"
#include "Utility/Log.h"
#include "Component/Jump.h"
#include "GameObject/GameObject.h"

REGISTER_STATE(TMF::PlayerJumpState, ("PlayerJumpState"));

namespace TMF
{
	void PlayerJumpState::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pJump = pLockOwner->GetComponent<Jump>();
			if (auto pLockJump = m_pJump.lock())
			{
			
			}
		}
	}
	void PlayerJumpState::OnEnter()
	{
		//Log::Info("%s", "Enter PlayerJumpState");
	}
	void PlayerJumpState::OnUpdate()
	{

	}
	void PlayerJumpState::OnExit()
	{

	}
}
