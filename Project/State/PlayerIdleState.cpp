#include "PlayerIdleState.h"

#include "StateRegister.h"
#include "Utility/Log.h"

REGISTER_STATE(TMF::PlayerIdleState, "PlayerIdleState");

namespace TMF
{
	PlayerIdleState::PlayerIdleState()
	{

	}
	PlayerIdleState::~PlayerIdleState()
	{

	}
	void PlayerIdleState::OnEnter()
	{
		//std::string enter = { "PlayerIdleStateEnter\n" };
		//Log::Info("%s",enter.c_str());
	}
	void PlayerIdleState::OnUpdate()
	{

	}
	void PlayerIdleState::OnExit()
	{

	}
}
