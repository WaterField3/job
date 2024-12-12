#pragma once
#include "State.h"

#include <memory>

namespace TMF
{
	class EnemyMoveState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnExit() override;

	private:

	};
}

