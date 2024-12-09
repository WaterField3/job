#pragma once
#include "State.h"

#include <memory>

namespace TMF
{
	class PlayerJump;
	class PlayerJumpState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnExit() override;

	private:
		std::unique_ptr<PlayerJump> m_pPlayerJump;
	};
}

