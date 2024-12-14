#include "State.h"

#include "GameObject/GameObject.h"

namespace TMF
{
	State::State() : m_pEventSystem(std::make_unique<EventSystem>())
	{

	}
	void State::Initialize(std::weak_ptr<GameObject> pOwenr, std::weak_ptr<StateMachine> pAdministerStateMachine)
	{
		m_pOwner = pOwenr;
		m_pAdministratorStateMachine = pAdministerStateMachine;
		OnInitialize();
	}
	void State::Enter()
	{
		OnEnter();
	}
	void State::Update()
	{
		OnUpdate();
	}
	void State::LateUpdate()
	{
		OnLateUpdate();
	}
	void State::Exit()
	{
		OnExit();
	}
}
