#pragma once
#include "State.h"

namespace TMF
{
	class PlayerJumpState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnExit() override;
	};
}

