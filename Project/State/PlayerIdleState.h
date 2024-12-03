#pragma once

#include "State.h"

namespace TMF
{
	class PlayerIdleState : public State
	{
	public:
		PlayerIdleState();
		~PlayerIdleState();

		void OnEnter() override;
		void OnUpdate() override;
		void OnExit() override;
	};
}

