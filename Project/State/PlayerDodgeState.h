#pragma once
#include "State.h"

#include <memory>

namespace TMF
{
	class Dodge;
	class PlayerDodgeState : public State
	{
	public:
		PlayerDodgeState();
		~PlayerDodgeState();
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnExit() override;

	private:
		std::weak_ptr<Dodge> m_pDodge;
	};
}

