#include "State.h"

namespace TMF
{
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
