#pragma once

namespace TMF
{
	class State
	{
	public:
		State() {};
		virtual ~State() = default;
		void Enter();
		void Update();
		void Exit();

	protected:
		virtual void OnEnter() {};
		virtual void OnUpdate() {};
		virtual void OnExit() {};

	};
}

