#include "StateMachine.h"

#include "State/StateManager.h"

namespace TMF
{
	void StateMachine::ChangeState(std::string name)
	{
		if (m_pCurrentState)
		{
			m_pCurrentState->Exit();
		}
		m_pCurrentState = StateManager::Instance().GetState(name);
		if (m_pCurrentState)
		{
			m_pCurrentState->Enter();
		}
	}
	void StateMachine::Update()
	{
		m_pCurrentState->Update();
	}
}
