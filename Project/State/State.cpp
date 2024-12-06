#include "State.h"

#include "GameObject/GameObject.h"

#include "EventSystem/EventSystem.h"

namespace TMF
{
	State::State() : m_pEventSystem(std::make_unique<EventSystem>())
	{

	}
	void State::Initialize(std::weak_ptr<GameObject> pOwenr)
	{
		m_pOwner = pOwenr;
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
	void State::Exit()
	{
		OnExit();
	}
}
