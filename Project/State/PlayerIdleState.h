#pragma once

#include "State.h"

namespace TMF
{
	class PlayerMove;
	class PlayerIdleState : public State
	{
	public:
		PlayerIdleState();
		~PlayerIdleState();

		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnExit() override;

	private:
		std::unique_ptr<PlayerMove> m_pPlayerMove;

	};
}

