#pragma once
#include "State.h"

#include <memory>

namespace TMF
{
	class EnemyMove;
	class EnemyMoveState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnExit() override;

	private:
		std::weak_ptr<EnemyMove> m_pEnemyMove;
	};
}

