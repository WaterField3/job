#pragma once

#include <memory>
#include <string>

#include "State.h"

// コンポーネント化に使用

namespace TMF
{
	class StateMachine
	{
	private:
		std::unique_ptr<State> m_pCurrentState;

	public:
		template <typename TState>
		void ChangeState()
		{
			if (m_pCurrentState)
			{
				m_pCurrentState->Exit();
			}
			auto pState = std::make_unique<TState>();
			m_pCurrentState = std::move(pState);
			if (m_pCurrentState)
			{
				m_pCurrentState->Enter();
			}
		}
		void ChangeState(std::string name);

		void Update();
	};
}

