#pragma once
#include "State.h"

namespace TMF
{
	class Thruster;
	class PlayerAttackState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnExit() override;

	private:
		float m_endTime = 1.0f;
		float m_cancelTime = 1.0f;
		float m_timer = 0.0f;
		std::weak_ptr <Thruster> m_pThruster;
	};
}

