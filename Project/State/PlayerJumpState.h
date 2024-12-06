#pragma once
#include "State.h"

#include <memory>

namespace TMF
{
	class Jump;
	class PlayerJumpState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnExit() override;

	private:
		std::weak_ptr<Jump> m_pJump;
	};
}

