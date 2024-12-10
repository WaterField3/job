#pragma once

#include <memory>
#include <string>

#include "State.h"

// コンポーネント化に使用

namespace TMF
{
	class GameObject;
	class StateMachine : public std::enable_shared_from_this<StateMachine>
	{
	public:
		StateMachine(std::weak_ptr<GameObject> pOwner);
		~StateMachine();
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

	private:
		std::unique_ptr<State> m_pCurrentState;
		std::weak_ptr<GameObject> m_pOwner;
	};
}

