#pragma once

#include <memory>

namespace TMF
{
	class GameObject;
	class EventSystem;
	class State
	{
	public:
		State();
		virtual ~State() = default;

		void Initialize(std::weak_ptr<GameObject> pOwenr);
		void Enter();
		void Update();
		void Exit();

	protected:
		virtual void OnInitialize() {};
		virtual void OnEnter() {};
		virtual void OnUpdate() {};
		virtual void OnExit() {};


		std::weak_ptr<GameObject> m_pOwner;
		std::unique_ptr<EventSystem> m_pEventSystem;

	};
}

