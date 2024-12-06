#include "StateMachine.h"

#include "State/StateManager.h"
#include "Utility/Log.h"

namespace TMF
{
	StateMachine::StateMachine(std::weak_ptr<GameObject> pOwner)
	{
		m_pOwner = pOwner;
	}
	StateMachine::~StateMachine()
	{

	}
	void StateMachine::ChangeState(std::string name)
	{
		if (m_pCurrentState)
		{
			m_pCurrentState->Exit();
		}
		m_pCurrentState = StateManager::Instance().GetState(name);
		if (m_pCurrentState)
		{
			try
			{
				m_pCurrentState->Initialize(m_pOwner, shared_from_this());
			}
			catch (const std::exception& e)
			{
				Log::Info("%s", e.what());
			}
			m_pCurrentState->Enter();
		}
	}
	void StateMachine::Update()
	{
		m_pCurrentState->Update();
	}
}
